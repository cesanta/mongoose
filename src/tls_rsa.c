#include "sha256.h"
#include "tls.h"
#include "tls_rsa.h"
#include "util.h"

#if MG_TLS == MG_TLS_BUILTIN

/*
 * The RSA bigint backend below is derived from BearSSL's i31 RSA code.
 *
 * Copyright (c) 2016 Thomas Pornin <pornin@bolet.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#define BR_MAX_RSA_SIZE 4096
#define BR_MAX_RSA_FACTOR ((BR_MAX_RSA_SIZE + 64) >> 1)
#define BR_I31_WORDS(bits) (2 + (((bits) + 30) / 31))
#define BR_RSA_WORDS BR_I31_WORDS(BR_MAX_RSA_SIZE)
#define BR_RSA_FACTOR_WORDS BR_I31_WORDS(BR_MAX_RSA_FACTOR)
#define BR_64 1
#define MUL31(x, y) ((uint64_t) (x) * (uint64_t) (y))
#define MUL31_lo(x, y) (((uint32_t) (x) * (uint32_t) (y)) & 0x7fffffffU)
#define CCOPY br_ccopy

static uint32_t NOT(uint32_t ctl) { return ctl ^ 1; }
static uint32_t MUX(uint32_t ctl, uint32_t x, uint32_t y) {
  return y ^ (-ctl & (x ^ y));
}
static uint32_t EQ(uint32_t x, uint32_t y) {
  uint32_t q = x ^ y;
  return NOT((q | -q) >> 31);
}
static uint32_t NEQ(uint32_t x, uint32_t y) {
  uint32_t q = x ^ y;
  return (q | -q) >> 31;
}
static uint32_t GT(uint32_t x, uint32_t y) {
  uint32_t z = y - x;
  return (z ^ ((x ^ y) & (x ^ z))) >> 31;
}
#define GE(x, y) NOT(GT(y, x))
#define LT(x, y) GT(y, x)
static uint32_t BIT_LENGTH(uint32_t x) {
  uint32_t k, c;
  k = NEQ(x, 0);
  c = GT(x, 0xffff); x = MUX(c, x >> 16, x); k += c << 4;
  c = GT(x, 0x00ff); x = MUX(c, x >> 8, x); k += c << 3;
  c = GT(x, 0x000f); x = MUX(c, x >> 4, x); k += c << 2;
  c = GT(x, 0x0003); x = MUX(c, x >> 2, x); k += c << 1;
  k += GT(x, 0x0001);
  return k;
}
static void br_enc32be(void *dst, uint32_t x) {
  unsigned char *buf = (unsigned char *) dst;
  buf[0] = (unsigned char) (x >> 24);
  buf[1] = (unsigned char) (x >> 16);
  buf[2] = (unsigned char) (x >> 8);
  buf[3] = (unsigned char) x;
}
static void br_ccopy(uint32_t ctl, void *dst, const void *src, size_t len) {
  unsigned char *d = (unsigned char *) dst;
  const unsigned char *s = (const unsigned char *) src;
  while (len-- > 0) {
    uint32_t x = *s++, y = *d;
    *d++ = (unsigned char) MUX(ctl, x, y);
  }
}

static uint32_t br_i31_add(uint32_t *, const uint32_t *, uint32_t);
static uint32_t br_i31_sub(uint32_t *, const uint32_t *, uint32_t);
static uint32_t br_i31_bit_length(uint32_t *, size_t);
static void br_i31_decode(uint32_t *, const void *, size_t);
static void br_i31_encode(void *, size_t, const uint32_t *);
static uint32_t br_i31_ninv31(uint32_t);
static uint32_t br_divrem(uint32_t, uint32_t, uint32_t, uint32_t *);
static void br_i31_muladd_small(uint32_t *, uint32_t, const uint32_t *);
static void br_i31_reduce(uint32_t *, const uint32_t *, const uint32_t *);
static void br_i31_rshift(uint32_t *, int);
static void br_i31_decode_reduce(uint32_t *, const void *, size_t, const uint32_t *);
static void br_i31_mulacc(uint32_t *, const uint32_t *, const uint32_t *);
static void br_i31_montymul(uint32_t *, const uint32_t *, const uint32_t *, const uint32_t *, uint32_t);
static void br_i31_to_monty(uint32_t *, const uint32_t *);
static void br_i31_modpow(uint32_t *, const unsigned char *, size_t, const uint32_t *, uint32_t, uint32_t *, uint32_t *);
static void br_i31_zero(uint32_t *x, uint32_t bit_len) {
  *x++ = bit_len;
  memset(x, 0, ((bit_len + 31) >> 5) * sizeof *x);
}
static uint32_t br_rem(uint32_t hi, uint32_t lo, uint32_t d) {
  uint32_t r;
  br_divrem(hi, lo, d, &r);
  return r;
}
static uint32_t br_div(uint32_t hi, uint32_t lo, uint32_t d) {
  uint32_t r;
  return br_divrem(hi, lo, d, &r);
}
static uint32_t
br_i31_add(uint32_t *a, const uint32_t *b, uint32_t ctl)
{
	uint32_t cc;
	size_t u, m;

	cc = 0;
	m = (a[0] + 63) >> 5;
	for (u = 1; u < m; u ++) {
		uint32_t aw, bw, naw;

		aw = a[u];
		bw = b[u];
		naw = aw + bw + cc;
		cc = naw >> 31;
		a[u] = MUX(ctl, naw & (uint32_t)0x7FFFFFFF, aw);
	}
	return cc;
}
static uint32_t
br_i31_sub(uint32_t *a, const uint32_t *b, uint32_t ctl)
{
	uint32_t cc;
	size_t u, m;

	cc = 0;
	m = (a[0] + 63) >> 5;
	for (u = 1; u < m; u ++) {
		uint32_t aw, bw, naw;

		aw = a[u];
		bw = b[u];
		naw = aw - bw - cc;
		cc = naw >> 31;
		a[u] = MUX(ctl, naw & 0x7FFFFFFF, aw);
	}
	return cc;
}
static uint32_t
br_i31_bit_length(uint32_t *x, size_t xlen)
{
	uint32_t tw, twk;

	tw = 0;
	twk = 0;
	while (xlen -- > 0) {
		uint32_t w, c;

		c = EQ(tw, 0);
		w = x[xlen];
		tw = MUX(c, w, tw);
		twk = MUX(c, (uint32_t)xlen, twk);
	}
	return (twk << 5) + BIT_LENGTH(tw);
}
static void
br_i31_decode(uint32_t *x, const void *src, size_t len)
{
	const unsigned char *buf;
	size_t u, v;
	uint32_t acc;
	int acc_len;

	buf = (const unsigned char *) src;
	u = len;
	v = 1;
	acc = 0;
	acc_len = 0;
	while (u -- > 0) {
		uint32_t b;

		b = buf[u];
		acc |= (b << acc_len);
		acc_len += 8;
		if (acc_len >= 31) {
			x[v ++] = acc & (uint32_t)0x7FFFFFFF;
			acc_len -= 31;
			acc = b >> (8 - acc_len);
		}
	}
	if (acc_len != 0) {
		x[v ++] = acc;
	}
	x[0] = br_i31_bit_length(x + 1, v - 1);
}
static void
br_i31_encode(void *dst, size_t len, const uint32_t *x)
{
	unsigned char *buf;
	size_t k, xlen;
	uint32_t acc;
	int acc_len;

	xlen = (x[0] + 31) >> 5;
	if (xlen == 0) {
		memset(dst, 0, len);
		return;
	}
	buf = (unsigned char *)dst + len;
	k = 1;
	acc = 0;
	acc_len = 0;
	while (len != 0) {
		uint32_t w;

		w = (k <= xlen) ? x[k] : 0;
		k ++;
		if (acc_len == 0) {
			acc = w;
			acc_len = 31;
		} else {
			uint32_t z;

			z = acc | (w << acc_len);
			acc_len --;
			acc = w >> (31 - acc_len);
			if (len >= 4) {
				buf -= 4;
				len -= 4;
				br_enc32be(buf, z);
			} else {
				switch (len) {
				case 3:
					buf[-3] = (unsigned char)(z >> 16);
					/* fall through */
				case 2:
					buf[-2] = (unsigned char)(z >> 8);
					/* fall through */
				case 1:
					buf[-1] = (unsigned char)z;
					break;
				}
				return;
			}
		}
	}
}
static uint32_t
br_i31_ninv31(uint32_t x)
{
	uint32_t y;

	y = 2 - x;
	y *= 2 - y * x;
	y *= 2 - y * x;
	y *= 2 - y * x;
	y *= 2 - y * x;
	return MUX(x & 1, -y, 0) & 0x7FFFFFFF;
}
static uint32_t
br_divrem(uint32_t hi, uint32_t lo, uint32_t d, uint32_t *r)
{
	/* TODO: optimize this */
	uint32_t q;
	uint32_t ch, cf;
	int k;

	q = 0;
	ch = EQ(hi, d);
	hi = MUX(ch, 0, hi);
	for (k = 31; k > 0; k --) {
		int j;
		uint32_t w, ctl, hi2, lo2;

		j = 32 - k;
		w = (hi << j) | (lo >> k);
		ctl = GE(w, d) | (hi >> k);
		hi2 = (w - d) >> j;
		lo2 = lo - (d << k);
		hi = MUX(ctl, hi2, hi);
		lo = MUX(ctl, lo2, lo);
		q |= ctl << k;
	}
	cf = GE(lo, d) | hi;
	q |= cf;
	*r = MUX(cf, lo - d, lo);
	return q;
}
static void
br_i31_muladd_small(uint32_t *x, uint32_t z, const uint32_t *m)
{
	uint32_t m_bitlen;
	unsigned mblr;
	size_t u, mlen;
	uint32_t a0, a1, b0, hi, g, q, tb;
	uint32_t under, over;
	uint32_t cc;

	/*
	 * We can test on the modulus bit length since we accept to
	 * leak that length.
	 */
	m_bitlen = m[0];
	if (m_bitlen == 0) {
		return;
	}
	if (m_bitlen <= 31) {
		uint32_t lo;

		hi = x[1] >> 1;
		lo = (x[1] << 31) | z;
		x[1] = br_rem(hi, lo, m[1]);
		return;
	}
	mlen = (m_bitlen + 31) >> 5;
	mblr = (unsigned)m_bitlen & 31;

	/*
	 * Principle: we estimate the quotient (x*2^31+z)/m by
	 * doing a 64/32 division with the high words.
	 *
	 * Let:
	 *   w = 2^31
	 *   a = (w*a0 + a1) * w^N + a2
	 *   b = b0 * w^N + b2
	 * such that:
	 *   0 <= a0 < w
	 *   0 <= a1 < w
	 *   0 <= a2 < w^N
	 *   w/2 <= b0 < w
	 *   0 <= b2 < w^N
	 *   a < w*b
	 * I.e. the two top words of a are a0:a1, the top word of b is
	 * b0, we ensured that b0 is "full" (high bit set), and a is
	 * such that the quotient q = a/b fits on one word (0 <= q < w).
	 *
	 * If a = b*q + r (with 0 <= r < q), we can estimate q by
	 * doing an Euclidean division on the top words:
	 *   a0*w+a1 = b0*u + v  (with 0 <= v < b0)
	 * Then the following holds:
	 *   0 <= u <= w
	 *   u-2 <= q <= u
	 */
	hi = x[mlen];
	if (mblr == 0) {
		a0 = x[mlen];
		memmove(x + 2, x + 1, (mlen - 1) * sizeof *x);
		x[1] = z;
		a1 = x[mlen];
		b0 = m[mlen];
	} else {
		a0 = ((x[mlen] << (31 - mblr)) | (x[mlen - 1] >> mblr))
			& 0x7FFFFFFF;
		memmove(x + 2, x + 1, (mlen - 1) * sizeof *x);
		x[1] = z;
		a1 = ((x[mlen] << (31 - mblr)) | (x[mlen - 1] >> mblr))
			& 0x7FFFFFFF;
		b0 = ((m[mlen] << (31 - mblr)) | (m[mlen - 1] >> mblr))
			& 0x7FFFFFFF;
	}

	/*
	 * We estimate a divisor q. If the quotient returned by br_div()
	 * is g:
	 * -- If a0 == b0 then g == 0; we want q = 0x7FFFFFFF.
	 * -- Otherwise:
	 *    -- if g == 0 then we set q = 0;
	 *    -- otherwise, we set q = g - 1.
	 * The properties described above then ensure that the true
	 * quotient is q-1, q or q+1.
	 *
	 * Take care that a0, a1 and b0 are 31-bit words, not 32-bit. We
	 * must adjust the parameters to br_div() accordingly.
	 */
	g = br_div(a0 >> 1, a1 | (a0 << 31), b0);
	q = MUX(EQ(a0, b0), 0x7FFFFFFF, MUX(EQ(g, 0), 0, g - 1));

	/*
	 * We subtract q*m from x (with the extra high word of value 'hi').
	 * Since q may be off by 1 (in either direction), we may have to
	 * add or subtract m afterwards.
	 *
	 * The 'tb' flag will be true (1) at the end of the loop if the
	 * result is greater than or equal to the modulus (not counting
	 * 'hi' or the carry).
	 */
	cc = 0;
	tb = 1;
	for (u = 1; u <= mlen; u ++) {
		uint32_t mw, zw, xw, nxw;
		uint64_t zl;

		mw = m[u];
		zl = MUL31(mw, q) + cc;
		cc = (uint32_t)(zl >> 31);
		zw = (uint32_t)zl & (uint32_t)0x7FFFFFFF;
		xw = x[u];
		nxw = xw - zw;
		cc += nxw >> 31;
		nxw &= 0x7FFFFFFF;
		x[u] = nxw;
		tb = MUX(EQ(nxw, mw), tb, GT(nxw, mw));
	}

	/*
	 * If we underestimated q, then either cc < hi (one extra bit
	 * beyond the top array word), or cc == hi and tb is true (no
	 * extra bit, but the result is not lower than the modulus). In
	 * these cases we must subtract m once.
	 *
	 * Otherwise, we may have overestimated, which will show as
	 * cc > hi (thus a negative result). Correction is adding m once.
	 */
	over = GT(cc, hi);
	under = ~over & (tb | LT(cc, hi));
	br_i31_add(x, m, over);
	br_i31_sub(x, m, under);
}
static void
br_i31_reduce(uint32_t *x, const uint32_t *a, const uint32_t *m)
{
	uint32_t m_bitlen, a_bitlen;
	size_t mlen, alen, u;

	m_bitlen = m[0];
	mlen = (m_bitlen + 31) >> 5;

	x[0] = m_bitlen;
	if (m_bitlen == 0) {
		return;
	}

	/*
	 * If the source is shorter, then simply copy all words from a[]
	 * and zero out the upper words.
	 */
	a_bitlen = a[0];
	alen = (a_bitlen + 31) >> 5;
	if (a_bitlen < m_bitlen) {
		memcpy(x + 1, a + 1, alen * sizeof *a);
		for (u = alen; u < mlen; u ++) {
			x[u + 1] = 0;
		}
		return;
	}

	/*
	 * The source length is at least equal to that of the modulus.
	 * We must thus copy N-1 words, and input the remaining words
	 * one by one.
	 */
	memcpy(x + 1, a + 2 + (alen - mlen), (mlen - 1) * sizeof *a);
	x[mlen] = 0;
	for (u = 1 + alen - mlen; u > 0; u --) {
		br_i31_muladd_small(x, a[u], m);
	}
}
static void
br_i31_rshift(uint32_t *x, int count)
{
	size_t u, len;
	uint32_t r;

	len = (x[0] + 31) >> 5;
	if (len == 0) {
		return;
	}
	r = x[1] >> count;
	for (u = 2; u <= len; u ++) {
		uint32_t w;

		w = x[u];
		x[u - 1] = ((w << (31 - count)) | r) & 0x7FFFFFFF;
		r = w >> count;
	}
	x[len] = r;
}
static void
br_i31_decode_reduce(uint32_t *x,
	const void *src, size_t len, const uint32_t *m)
{
	uint32_t m_ebitlen, m_rbitlen;
	size_t mblen, k;
	const unsigned char *buf;
	uint32_t acc;
	int acc_len;

	/*
	 * Get the encoded bit length.
	 */
	m_ebitlen = m[0];

	/*
	 * Special case for an invalid (null) modulus.
	 */
	if (m_ebitlen == 0) {
		x[0] = 0;
		return;
	}

	/*
	 * Clear the destination.
	 */
	br_i31_zero(x, m_ebitlen);

	/*
	 * First decode directly as many bytes as possible. This requires
	 * computing the actual bit length.
	 */
	m_rbitlen = m_ebitlen >> 5;
	m_rbitlen = (m_ebitlen & 31) + (m_rbitlen << 5) - m_rbitlen;
	mblen = (m_rbitlen + 7) >> 3;
	k = mblen - 1;
	if (k >= len) {
		br_i31_decode(x, src, len);
		x[0] = m_ebitlen;
		return;
	}
	buf = (const unsigned char *) src;
	br_i31_decode(x, buf, k);
	x[0] = m_ebitlen;

	/*
	 * Input remaining bytes, using 31-bit words.
	 */
	acc = 0;
	acc_len = 0;
	while (k < len) {
		uint32_t v;

		v = buf[k ++];
		if (acc_len >= 23) {
			acc_len -= 23;
			acc <<= (8 - acc_len);
			acc |= v >> acc_len;
			br_i31_muladd_small(x, acc, m);
			acc = v & (0xFF >> (8 - acc_len));
		} else {
			acc = (acc << 8) | v;
			acc_len += 8;
		}
	}

	/*
	 * We may have some bits accumulated. We then perform a shift to
	 * be able to inject these bits as a full 31-bit word.
	 */
	if (acc_len != 0) {
		acc = (acc | (x[1] << acc_len)) & 0x7FFFFFFF;
		br_i31_rshift(x, 31 - acc_len);
		br_i31_muladd_small(x, acc, m);
	}
}
static void
br_i31_mulacc(uint32_t *d, const uint32_t *a, const uint32_t *b)
{
	size_t alen, blen, u;
	uint32_t dl, dh;

	alen = (a[0] + 31) >> 5;
	blen = (b[0] + 31) >> 5;

	/*
	 * We want to add the two bit lengths, but these are encoded,
	 * which requires some extra care.
	 */
	dl = (a[0] & 31) + (b[0] & 31);
	dh = (a[0] >> 5) + (b[0] >> 5);
	d[0] = (dh << 5) + dl + (~(uint32_t)(dl - 31) >> 31);

	for (u = 0; u < blen; u ++) {
		uint32_t f;
		size_t v;

		/*
		 * Carry always fits on 31 bits; we want to keep it in a
		 * 32-bit register on 32-bit architectures (on a 64-bit
		 * architecture, cast down from 64 to 32 bits means
		 * clearing the high bits, which is not free; on a 32-bit
		 * architecture, the same operation really means ignoring
		 * the top register, which has negative or zero cost).
		 */
#if BR_64
		uint64_t cc;
#else
		uint32_t cc;
#endif

		f = b[1 + u];
		cc = 0;
		for (v = 0; v < alen; v ++) {
			uint64_t z;

			z = (uint64_t)d[1 + u + v] + MUL31(f, a[1 + v]) + cc;
			cc = z >> 31;
			d[1 + u + v] = (uint32_t)z & 0x7FFFFFFF;
		}
		d[1 + u + alen] = (uint32_t)cc;
	}
}
static void
br_i31_montymul(uint32_t *d, const uint32_t *x, const uint32_t *y,
	const uint32_t *m, uint32_t m0i)
{
	/*
	 * Each outer loop iteration computes:
	 *   d <- (d + xu*y + f*m) / 2^31
	 * We have xu <= 2^31-1 and f <= 2^31-1.
	 * Thus, if d <= 2*m-1 on input, then:
	 *   2*m-1 + 2*(2^31-1)*m <= (2^32)*m-1
	 * and the new d value is less than 2*m.
	 *
	 * We represent d over 31-bit words, with an extra word 'dh'
	 * which can thus be only 0 or 1.
	 */
	size_t len, len4, u, v;
	uint32_t dh;

	len = (m[0] + 31) >> 5;
	len4 = len & ~(size_t)3;
	br_i31_zero(d, m[0]);
	dh = 0;
	for (u = 0; u < len; u ++) {
		/*
		 * The carry for each operation fits on 32 bits:
		 *   d[v+1] <= 2^31-1
		 *   xu*y[v+1] <= (2^31-1)*(2^31-1)
		 *   f*m[v+1] <= (2^31-1)*(2^31-1)
		 *   r <= 2^32-1
		 *   (2^31-1) + 2*(2^31-1)*(2^31-1) + (2^32-1) = 2^63 - 2^31
		 * After division by 2^31, the new r is then at most 2^32-1
		 *
		 * Using a 32-bit carry has performance benefits on 32-bit
		 * systems; however, on 64-bit architectures, we prefer to
		 * keep the carry (r) in a 64-bit register, thus avoiding some
		 * "clear high bits" operations.
		 */
		uint32_t f, xu;
#if BR_64
		uint64_t r;
#else
		uint32_t r;
#endif

		xu = x[u + 1];
		f = MUL31_lo((d[1] + MUL31_lo(x[u + 1], y[1])), m0i);

		r = 0;
		for (v = 0; v < len4; v += 4) {
			uint64_t z;

			z = (uint64_t)d[v + 1] + MUL31(xu, y[v + 1])
				+ MUL31(f, m[v + 1]) + r;
			r = z >> 31;
			d[v + 0] = (uint32_t)z & 0x7FFFFFFF;
			z = (uint64_t)d[v + 2] + MUL31(xu, y[v + 2])
				+ MUL31(f, m[v + 2]) + r;
			r = z >> 31;
			d[v + 1] = (uint32_t)z & 0x7FFFFFFF;
			z = (uint64_t)d[v + 3] + MUL31(xu, y[v + 3])
				+ MUL31(f, m[v + 3]) + r;
			r = z >> 31;
			d[v + 2] = (uint32_t)z & 0x7FFFFFFF;
			z = (uint64_t)d[v + 4] + MUL31(xu, y[v + 4])
				+ MUL31(f, m[v + 4]) + r;
			r = z >> 31;
			d[v + 3] = (uint32_t)z & 0x7FFFFFFF;
		}
		for (; v < len; v ++) {
			uint64_t z;

			z = (uint64_t)d[v + 1] + MUL31(xu, y[v + 1])
				+ MUL31(f, m[v + 1]) + r;
			r = z >> 31;
			d[v] = (uint32_t)z & 0x7FFFFFFF;
		}

		/*
		 * Since the new dh can only be 0 or 1, the addition of
		 * the old dh with the carry MUST fit on 32 bits, and
		 * thus can be done into dh itself.
		 */
		dh += (uint32_t) r;
		d[len] = dh & 0x7FFFFFFF;
		dh >>= 31;
	}

	/*
	 * We must write back the bit length because it was overwritten in
	 * the loop (not overwriting it would require a test in the loop,
	 * which would yield bigger and slower code).
	 */
	d[0] = m[0];

	/*
	 * d[] may still be greater than m[] at that point; notably, the
	 * 'dh' word may be non-zero.
	 */
	br_i31_sub(d, m, NEQ(dh, 0) | NOT(br_i31_sub(d, m, 0)));
}
static void
br_i31_to_monty(uint32_t *x, const uint32_t *m)
{
	uint32_t k;

	for (k = (m[0] + 31) >> 5; k > 0; k --) {
		br_i31_muladd_small(x, 0, m);
	}
}
static void
br_i31_modpow(uint32_t *x,
	const unsigned char *e, size_t elen,
	const uint32_t *m, uint32_t m0i, uint32_t *t1, uint32_t *t2)
{
	size_t mlen;
	uint32_t k;

	/*
	 * 'mlen' is the length of m[] expressed in bytes (including
	 * the "bit length" first field).
	 */
	mlen = ((m[0] + 63) >> 5) * sizeof m[0];

	/*
	 * Throughout the algorithm:
	 * -- t1[] is in Montgomery representation; it contains x, x^2,
	 * x^4, x^8...
	 * -- The result is accumulated, in normal representation, in
	 * the x[] array.
	 * -- t2[] is used as destination buffer for each multiplication.
	 *
	 * Note that there is no need to call br_i32_from_monty().
	 */
	memcpy(t1, x, mlen);
	br_i31_to_monty(t1, m);
	br_i31_zero(x, m[0]);
	x[1] = 1;
	for (k = 0; k < ((uint32_t)elen << 3); k ++) {
		uint32_t ctl;

		ctl = (e[elen - 1 - (k >> 3)] >> (k & 7)) & 1;
		br_i31_montymul(t2, x, t1, m, m0i);
		CCOPY(ctl, x, t2, mlen);
		br_i31_montymul(t2, t1, t1, m, m0i);
		memcpy(t1, t2, mlen);
	}
}

static size_t rsa_trim(const uint8_t **p, size_t n) {
  while (n > 0 && **p == 0) (*p)++, n--;
  return n;
}

static int rsa_i31_len(size_t nbytes) {
  int z = (int) (nbytes * 8U);
  int n = 1;
  while (z > 0) z -= 31, n++;
  return n + (n & 1);
}

static int rsa_fail(uint8_t *out, size_t outsz) {
  mg_bzero(out, outsz);
  return -1;
}

static int rsa_i31_eq(const uint32_t *a, const uint32_t *b) {
  uint32_t r = a[0] ^ b[0];
  size_t u, n = (a[0] + 31) >> 5;
  for (u = 1; u <= n; u++) r |= a[u] ^ b[u];
  return r == 0;
}

int mg_rsa_mod_pow(const uint8_t *mod, size_t modsz, const uint8_t *exp,
                   size_t expsz, const uint8_t *msg, size_t msgsz,
                   uint8_t *out, size_t outsz) {
  uint32_t tmp[4 * BR_RSA_WORDS], *m = tmp, *x, *t1, *t2, m0i;
  int fwlen;
  if (out == NULL) return -1;
  mg_bzero(out, outsz);
  if (mod == NULL || exp == NULL || msg == NULL) return -1;
  modsz = rsa_trim(&mod, modsz);
  expsz = rsa_trim(&exp, expsz);
  if (modsz < 256 || modsz > (BR_MAX_RSA_SIZE >> 3) || expsz == 0 ||
      expsz > modsz || outsz != modsz || msgsz != modsz) {
    return -1;
  }
  if (memcmp(msg, mod, modsz) >= 0) return -1;
  fwlen = rsa_i31_len(modsz);
  if (4 * (size_t) fwlen > sizeof(tmp) / sizeof(tmp[0])) return -1;
  x = m + fwlen, t1 = x + fwlen, t2 = t1 + fwlen;
  br_i31_decode(m, mod, modsz);
  m0i = br_i31_ninv31(m[1]);
  if ((m0i & 1) == 0) return rsa_fail((uint8_t *) tmp, sizeof(tmp));
  br_i31_decode_reduce(x, msg, msgsz, m);
  br_i31_modpow(x, exp, expsz, m, m0i, t1, t2);
  br_i31_encode(out, outsz, x);
  mg_bzero((uint8_t *) tmp, sizeof(tmp));
  return 0;
}

int mg_rsa_crt_sign(const uint8_t *em, size_t em_len,
                    const uint8_t *dP, size_t dP_len,
                    const uint8_t *dQ, size_t dQ_len,
                    const uint8_t *p, size_t p_len,
                    const uint8_t *q, size_t q_len,
                    const uint8_t *qInv, size_t qInv_len,
                    uint8_t *signature, size_t sig_len) {
  uint32_t tmp[8 * BR_RSA_FACTOR_WORDS], *mq = tmp, *s2, *mp, *s1, *t1, *t2;
  uint32_t *c1, *c2;
  uint32_t p0i, q0i;
  size_t plen, qlen, xlen = sig_len, u;
  int fwlen, ok = 0;
  uint8_t x[BR_MAX_RSA_SIZE >> 3];
  if (signature == NULL) return -1;
  mg_bzero(signature, sig_len);
  if (em == NULL || dP == NULL || dQ == NULL || p == NULL || q == NULL ||
      qInv == NULL) {
    return -1;
  }
  p_len = rsa_trim(&p, p_len);
  q_len = rsa_trim(&q, q_len);
  dP_len = rsa_trim(&dP, dP_len);
  dQ_len = rsa_trim(&dQ, dQ_len);
  qInv_len = rsa_trim(&qInv, qInv_len);
  if (p_len < 128 || q_len < 128 || p_len > (BR_MAX_RSA_FACTOR >> 3) ||
      q_len > (BR_MAX_RSA_FACTOR >> 3) || dP_len == 0 || dQ_len == 0 ||
      qInv_len == 0 || dP_len > p_len || dQ_len > q_len ||
      qInv_len > p_len || xlen > sizeof(x) || em_len == 0 ||
      em_len > xlen) {
    return -1;
  }
  plen = p_len, qlen = q_len;
  fwlen = rsa_i31_len(plen > qlen ? plen : qlen);
  if (8 * fwlen > (int) (sizeof(tmp) / sizeof(tmp[0]))) return -1;
  s2 = mq + fwlen, mp = mq + 2 * fwlen, s1 = mq + 3 * fwlen;
  t1 = mq + 4 * fwlen, t2 = mq + 5 * fwlen;
  c1 = mq + 6 * fwlen, c2 = mq + 7 * fwlen;
  memset(x, 0, xlen - em_len);
  memcpy(x + xlen - em_len, em, em_len);
  br_i31_decode(mq, q, qlen);
  br_i31_decode(mp, p, plen);
  q0i = br_i31_ninv31(mq[1]);
  p0i = br_i31_ninv31(mp[1]);
  if (((p0i & q0i) & 1) == 0) goto done;
  memset(t2, 0, 2 * (size_t) fwlen * sizeof(*t2));
  t2[0] = mq[0];
  br_i31_mulacc(t2, mq, mp);
  if (xlen != ((t2[0] + 7) >> 3)) goto done;
  br_i31_encode(signature, xlen, t2);
  for (u = xlen, ok = 0; u > 0; u--) {
    uint32_t wn = signature[u - 1], wx = x[u - 1];
    ok = (int) (((wx - (wn + (uint32_t) ok)) >> 8) & 1);
  }
  if (!ok) goto done;
  ok = 0;
  br_i31_decode_reduce(s2, x, xlen, mq);
  br_i31_modpow(s2, dQ, dQ_len, mq, q0i, t1, t2);
  br_i31_decode_reduce(s1, x, xlen, mp);
  br_i31_modpow(s1, dP, dP_len, mp, p0i, t1, t2);
  memcpy(c1, s1, (size_t) fwlen * sizeof(*s1));
  memcpy(c2, s2, (size_t) fwlen * sizeof(*s2));
  br_i31_reduce(t2, s2, mp);
  br_i31_add(s1, mp, br_i31_sub(s1, t2, 1));
  br_i31_to_monty(s1, mp);
  br_i31_decode_reduce(t1, qInv, qInv_len, mp);
  br_i31_montymul(t2, s1, t1, mp, p0i);
  br_i31_mulacc(s2, mq, t2);
  br_i31_reduce(t1, s2, mq);
  ok = rsa_i31_eq(t1, c2);
  br_i31_decode(mp, p, plen);
  br_i31_reduce(t1, s2, mp);
  ok &= rsa_i31_eq(t1, c1);
  if (!ok) goto done;
  ok = 0;
  br_i31_encode(signature, sig_len, s2);
  ok = 1;
done:
  mg_bzero((uint8_t *) tmp, sizeof(tmp));
  mg_bzero(x, sizeof(x));
  if (!ok) return rsa_fail(signature, sig_len);
  return 0;
}

// MGF1-SHA256 RFC 8017 B.2.1
static void mgf1_sha256(const uint8_t *seed, size_t seed_len,
                            uint8_t *mask,  size_t mask_len) {
  uint8_t cnt[4];   // I2OSP(counter, 4), big-endian
  uint8_t tmp[32];  // one SHA-256 output block
  uint32_t counter = 0;
  size_t off = 0;
  mg_sha256_ctx ctx;
  while (off < mask_len) {
    size_t n = mask_len - off;
    if (n > 32) n = 32;
    br_enc32be(cnt, counter);
    mg_sha256_init(&ctx);
    mg_sha256_update(&ctx, seed, seed_len);
    mg_sha256_update(&ctx, cnt, 4);
    mg_sha256_final(tmp, &ctx);
    memcpy(mask + off, tmp, n);
    off += n;
    ++counter;
  }
}

// Full RSASSA-PSS-VERIFY for rsa_pss_rsae_sha256 (RFC 8017 9.1.2)
// em      - output of RSA public-key primitive (nlen bytes)
// nlen    - modulus byte length (66 to 512)
// mhash   - SHA-256 of the TLS 1.3 signed content (tls->sighash, 32 bytes)
static bool pss_verify_sha256(const uint8_t *em, size_t nlen, const uint8_t *mhash) {
  // TLS 1.3: H_len = salt_len = 32
  uint8_t db[479];          // unmasked DB after MGF1 XOR
  uint8_t dbmask[479];      // MGF1-SHA256(H, db_len); nlen - hLen - 1 = 479
  uint8_t Hprime[32];       // SHA-256(0x00*8 || mhash || salt)
  uint8_t Mprime[8 + 32 + 32]; // 0*8 || mhash || salt
  const uint8_t *H;
  const uint8_t *salt;
  size_t db_len;
  size_t i;
  uint8_t bad;
  
  if (nlen < 32 + 32 + 2 || nlen - 32 - 1 > sizeof(db)) return -1;
  if (em[nlen - 1] != 0xbc) return false;
  if (em[0] & 0x80) return false;
  db_len = nlen - 32 - 1;
  H = em + db_len;
  mgf1_sha256(H, 32, dbmask, db_len);
  for (i = 0; i < db_len; i++) db[i] = em[i] ^ dbmask[i];
  db[0] &= 0x7f; // DB = 0x00^(db_len-salt_len-1) || 0x01 || salt(salt_Len)
  bad = 0;
  for (i = 0; i < db_len - 32 - 1; i++) bad |= db[i];
  bad |= db[db_len - 32 - 1] ^ 0x01;
  if (bad != 0) return false;
  salt = db + db_len - 32;
  memset(Mprime, 0, 8);
  memcpy(Mprime + 8, mhash, 32);
  memcpy(Mprime + 8 + 32, salt, 32);
  mg_sha256(Hprime, Mprime, sizeof(Mprime));
  bad = 0;
  for (i = 0; i < 32; i++) bad |= Hprime[i] ^ H[i];
  return (bad == 0);
}

bool mg_rsa_verify(const uint8_t *em, size_t nlen, const uint8_t *mhash) {
  return pss_verify_sha256(em, nlen, mhash);
}

// Full RSASSA-PKCS1-v1_5-VERIFY for rsa_pkcs1_sha256/sha384 (RFC 8017 9.2)
// em      - output of RSA public-key primitive (nlen bytes)
// nlen    - modulus byte length
// hash    - SHA-256 or SHA-384 of the certificate TBS data
// hashlen - hash length, 32 for SHA-256 or 48 for SHA-384
static bool pkcs1_v15_verify(const uint8_t *em, size_t nlen,
                             const uint8_t *hash, size_t hashlen) {
  static const uint8_t sha256_di[] = {
      0x30, 0x31, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
      0x65, 0x03, 0x04, 0x02, 0x01, 0x05, 0x00, 0x04, 0x20};
  static const uint8_t sha384_di[] = {
      0x30, 0x41, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
      0x65, 0x03, 0x04, 0x02, 0x02, 0x05, 0x00, 0x04, 0x30};
  const uint8_t *di = hashlen == 32 ? sha256_di : sha384_di;
  size_t disz = hashlen == 32 ? sizeof(sha256_di) : sizeof(sha384_di);
  size_t pslen, i;
  uint8_t bad = 0;

  if (hashlen != 32 && hashlen != 48) return false;
  if (nlen < 3 + 8 + disz + hashlen) return false;
  pslen = nlen - 3 - disz - hashlen;

  bad |= (uint8_t) (em[0] | (em[1] ^ 0x01));
  for (i = 0; i < pslen; i++) bad |= (uint8_t) (em[2 + i] ^ 0xff);
  bad |= em[2 + pslen];
  for (i = 0; i < disz; i++) bad |= (uint8_t) (em[3 + pslen + i] ^ di[i]);
  for (i = 0; i < hashlen; i++)
    bad |= (uint8_t) (em[3 + pslen + disz + i] ^ hash[i]);

  return bad == 0;
}

bool mg_rsa_pkcs_verify(const uint8_t *em, size_t nlen, const uint8_t *hash,
                        size_t hashlen) {
  return pkcs1_v15_verify(em, nlen, hash, hashlen);
}

#endif /* MG_TLS == MG_TLS_BUILTIN */
