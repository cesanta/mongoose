// P-256 for Cortex-M4/M7/M33, https://github.com/Emill/P256-Cortex-M4
// commit ecd3ec2
// Enabled by MG_P256=MG_P256_LENNGREN. Provides mg_p256_*(), see tls_p256.h
//
// Copyright (c) 2017-2021 Emil Lenngren
// Copyright (c) 2021 Shortcut Labs AB
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Mongoose changes: the library's C code, header, config and GCC assembly are
// merged into one file, the assembly is a top-level __asm__ without .end.
// Trimmed with unifdef to keygen, ECDH, sign and verify. Uses the no-FPU field
// multiplication and the constant RAM access pattern table lookup (safe with a
// data cache). Internal C functions are static, casts added in slide_257().
// mg_p256_*() wrappers are at the end

#include "tls_p256.h"
#include "util.h"

#if MG_P256 == MG_P256_LENNGREN && \
    (MG_TLS == MG_TLS_BUILTIN || defined(MG_OTA_PUBLIC_KEY))

typedef const uint32_t (*constarr)[8];

struct FGInteger {
    // To get the value this struct represents,
    // interpret signed_value as a two's complement 288-bit little endian integer,
    // and negate if flip_sign is -1
    int flip_sign; // 0 or -1
    uint32_t signed_value[9]; // of 288 bits, 257 are useful (top 31 bits are sign-extended from bit 256)
};

struct SignPrecomp {  // p256_sign_step1() result
    uint32_t r[8];
    uint32_t k_inv[8];
};

struct XYInteger {
    // To get the value this struct represents,
    // interpret signed_value as an unsigned 288-bit little endian integer,
    // and negate if flip_sign is -1
    int flip_sign; // 0 or -1
    uint32_t value[8]; // unsigned value, 0 <= value < P256_order
};

int P256_divsteps2_31(int delta, uint32_t f, uint32_t g, uint32_t res_matrix[4]);
void P256_matrix_mul_fg_9(uint32_t a, uint32_t b, const struct FGInteger fg[2], struct FGInteger *res);
void P256_matrix_mul_mod_n(uint32_t a, uint32_t b, const struct XYInteger xy[2], struct XYInteger *res);

void P256_to_montgomery(uint32_t aR[8], const uint32_t a[8]);
void P256_from_montgomery(uint32_t a[8], const uint32_t aR[8]);
bool P256_check_range_p(const uint32_t a[8]);

bool P256_check_range_n(const uint32_t a[8]);
void P256_mul_mod_n(uint32_t res[8], const uint32_t a[8], const uint32_t b[8]);
void P256_add_mod_n(uint32_t res[8], const uint32_t a[8], const uint32_t b[8]);
void P256_reduce_mod_n_32bytes(uint32_t res[8], const uint32_t a[8]);

void P256_select_point(uint32_t (*output)[8], uint32_t* table, uint32_t num_coordinates, uint32_t index);

void P256_jacobian_to_affine(uint32_t affine_mont_x[8], uint32_t affine_mont_y[8], const uint32_t jacobian_mont[3][8]);
bool P256_point_is_on_curve(const uint32_t x_mont[8], const uint32_t y_mont[8]);
void P256_double_j(uint32_t jacobian_point_out[3][8], const uint32_t jacobian_point_in[3][8]);
void P256_add_sub_j(uint32_t jacobian_point1[3][8], const uint32_t (*point2)[8], bool is_sub, bool p2_is_affine);
bool P256_verify_last_step(const uint32_t r[8], const uint32_t jacobian_point[3][8]);

void P256_negate_mod_p_if(uint32_t out[8], const uint32_t in[8], uint32_t should_negate);
void P256_negate_mod_n_if(uint32_t out[8], const uint32_t in[8], uint32_t should_negate);

extern uint32_t P256_order[9];

static const uint32_t one_montgomery[8] = {1, 0, 0, 0xffffffff, 0xffffffff, 0xffffffff, 0xfffffffe, 0};

// This table contains 1G, 3G, 5G, ... 15G in affine coordinates in montgomery form
static const uint32_t p256_basepoint_precomp[8][2][8] = {
{{0x18a9143c, 0x79e730d4, 0x5fedb601, 0x75ba95fc, 0x77622510, 0x79fb732b, 0xa53755c6, 0x18905f76},
{0xce95560a, 0xddf25357, 0xba19e45c, 0x8b4ab8e4, 0xdd21f325, 0xd2e88688, 0x25885d85, 0x8571ff18}},
{{0x4eebc127, 0xffac3f90, 0x87d81fb, 0xb027f84a, 0x87cbbc98, 0x66ad77dd, 0xb6ff747e, 0x26936a3f},
{0xc983a7eb, 0xb04c5c1f, 0x861fe1a, 0x583e47ad, 0x1a2ee98e, 0x78820831, 0xe587cc07, 0xd5f06a29}},
{{0xc45c61f5, 0xbe1b8aae, 0x94b9537d, 0x90ec649a, 0xd076c20c, 0x941cb5aa, 0x890523c8, 0xc9079605},
{0xe7ba4f10, 0xeb309b4a, 0xe5eb882b, 0x73c568ef, 0x7e7a1f68, 0x3540a987, 0x2dd1e916, 0x73a076bb}},
{{0xa0173b4f, 0x746354e, 0xd23c00f7, 0x2bd20213, 0xc23bb08, 0xf43eaab5, 0xc3123e03, 0x13ba5119},
{0x3f5b9d4d, 0x2847d030, 0x5da67bdd, 0x6742f2f2, 0x77c94195, 0xef933bdc, 0x6e240867, 0xeaedd915}},
{{0x264e20e8, 0x75c96e8f, 0x59a7a841, 0xabe6bfed, 0x44c8eb00, 0x2cc09c04, 0xf0c4e16b, 0xe05b3080},
{0xa45f3314, 0x1eb7777a, 0xce5d45e3, 0x56af7bed, 0x88b12f1a, 0x2b6e019a, 0xfd835f9b, 0x86659cd}},
{{0x6245e404, 0xea7d260a, 0x6e7fdfe0, 0x9de40795, 0x8dac1ab5, 0x1ff3a415, 0x649c9073, 0x3e7090f1},
{0x2b944e88, 0x1a768561, 0xe57f61c8, 0x250f939e, 0x1ead643d, 0xc0daa89, 0xe125b88e, 0x68930023}},
{{0x4b2ed709, 0xccc42563, 0x856fd30d, 0xe356769, 0x559e9811, 0xbcbcd43f, 0x5395b759, 0x738477ac},
{0xc00ee17f, 0x35752b90, 0x742ed2e3, 0x68748390, 0xbd1f5bc1, 0x7cd06422, 0xc9e7b797, 0xfbc08769}},
{{0xbc60055b, 0x72bcd8b7, 0x56e27e4b, 0x3cc23ee, 0xe4819370, 0xee337424, 0xad3da09, 0xe2aa0e43},
{0x6383c45d, 0x40b8524f, 0x42a41b25, 0xd7663554, 0x778a4797, 0x64efa6de, 0x7079adf4, 0x2042170a}}
};

// This contains two tables, 8 points each in affine coordinates in montgomery form
// The first table contains these points:
// (2^192 - 2^128 - 2^64 - 1)G
// (2^192 - 2^128 - 2^64 + 1)G
// (2^192 - 2^128 + 2^64 - 1)G
// (2^192 - 2^128 + 2^64 + 1)G
// (2^192 + 2^128 - 2^64 - 1)G
// (2^192 + 2^128 - 2^64 + 1)G
// (2^192 + 2^128 + 2^64 - 1)G
// (2^192 + 2^128 + 2^64 + 1)G
// The second table contains the same points multiplied by 2^32
static const uint32_t p256_basepoint_precomp2[2][8][2][8] =
{
{
{{0x670844e0, 0x52d8a7c9, 0xef68a29d, 0xe33bdc, 0x4bdb7361, 0xf3d2848, 0x91c5304d, 0x5222c821},
{0xdf73fc25, 0xea6d2944, 0x255c81b, 0xa04c0f55, 0xefe488a8, 0x29acdc97, 0x80a560de, 0xbe2e158f}},
{{0x2b13e673, 0xfc8511ee, 0xd103ed24, 0xffc58dee, 0xea7e99b8, 0x1022523a, 0x4afc8a17, 0x8f43ea39},
{0xc5f33d0b, 0x8f4e2dbc, 0xd0aa1681, 0x3bc099fa, 0x79ff9df1, 0xffbb7b41, 0xd58b57c4, 0x180de09d}},
{{0x8bd1cda5, 0x56430752, 0x8e05eda5, 0x1807577f, 0x956896e9, 0x99c699b, 0xf1f0efb5, 0x83d6093d},
{0xed97061c, 0xef5af17e, 0x30d4c3c, 0x35b977b8, 0x49229439, 0x81fa75a2, 0xa0b6d35d, 0xf5a22070}},
{{0x74f81cf1, 0x814c5365, 0x120065b, 0xe30baff7, 0x15132621, 0x80ae1256, 0x36a80788, 0x16d2b8cb},
{0xecc50bca, 0x33d14697, 0x17aedd21, 0x19a9dfb0, 0xedc3f766, 0x523fbcc7, 0xb2cf5afd, 0x9c4de6dd}},
{{0xcf0d9f6d, 0x5305a9e6, 0x81a9b021, 0x5839172f, 0x75c687cf, 0xcca7a4dd, 0x844be22f, 0x36d59b3e},
{0x111a53e9, 0xcace7e62, 0xf063f3a1, 0x91c843d4, 0xda812da, 0xbf77e5f0, 0x437f3176, 0xe64af9c}},
{{0xcf07517d, 0xdbd568bb, 0xba6830b9, 0x2f1afba2, 0xe6c4c2a6, 0x15b6807c, 0xe4966aef, 0x91c7eabc},
{0xd6b2b6e6, 0x716dea1b, 0x19f85b4b, 0x248c43d1, 0x4a315e2a, 0x16dcfd60, 0xc72b3d0b, 0x15fdd303}},
{{0x42b7dfd5, 0xe40bf9f4, 0x2d934f2a, 0x673689f3, 0x30a6f50b, 0x8314beb4, 0x976ec64e, 0xd17af2bc},
{0x1ee7ddf1, 0x39f66c4f, 0x68ea373c, 0x7f68e18b, 0x53d0b186, 0x5166c1f2, 0x7be58f14, 0x95dda601}},
{{0x42913074, 0xd5ae356, 0x48a542b1, 0x55491b27, 0xb310732a, 0x469ca665, 0x5f1a4cc1, 0x29591d52},
{0xb84f983f, 0xe76f5b6b, 0x9f5f84e1, 0xbe7eef41, 0x80baa189, 0x1200d496, 0x18ef332c, 0x6376551f}}
},
{
{{0x7c4e54f5, 0xb9e5cbc0, 0xe1410e34, 0xc53a1a17, 0xec454425, 0x3e199130, 0x1700902e, 0xb029c97e},
{0x786423b6, 0x2de66e11, 0xb41a95be, 0x262dc914, 0x451b683, 0x51766abd, 0x85bb6fb1, 0x55ad5f34}},
{{0x9066cb79, 0x74f4f1c, 0x30c8b94e, 0x1ab31bd6, 0xd74275b3, 0x6d3f012f, 0x9ddcce40, 0xa214d0b1},
{0xd165050a, 0x24aedf74, 0xe0e5dc3e, 0x95f17ece, 0xd9224456, 0x6ada9cda, 0x2dd60eea, 0x1fadb2d1}},
{{0xe20cfb9b, 0xa3d83091, 0xba76e0cb, 0xae79c975, 0xc8858a6e, 0xa5f2a588, 0x874a3168, 0xe897a5f4},
{0x7d48f096, 0xf6c1ef40, 0xc35b132c, 0x1f9c516b, 0x53c479fd, 0xe1040f91, 0x9df06743, 0x60e881f}},
{{0x52a90e51, 0x9e0ad72, 0x38c50a96, 0xb7e66ea3, 0x7d997770, 0xab32ad05, 0x445671cb, 0xceaffe2},
{0x5d37cc99, 0xdfbe753c, 0xe0fea2d5, 0x95d068cc, 0x4dd77cb6, 0x1e37cdda, 0x55530688, 0x88c5a4bb}},
{{0xc7744f1, 0x3413f033, 0xbc816702, 0x23c05c89, 0x1192b5ac, 0x2322ee9a, 0x373180bb, 0xc1636a0},
{0xbdde0207, 0xfe2f3d4, 0xc23578d8, 0xe1a093a, 0xc888ead, 0x6e5f0d1, 0x52a2b660, 0x9ca285a5}},
{{0xce923964, 0xdae76995, 0xa34c7993, 0xcc96493a, 0xea73d9e7, 0xd19b5144, 0x311e6e34, 0x4a5c263},
{0xd9a2a443, 0x7db5b32b, 0x2cfd960c, 0x3754bd33, 0xa430f15, 0xc5bcc98, 0xd9a94574, 0x5651201f}},
{{0xfc0418fe, 0xebdd8921, 0x34e20036, 0x37015b39, 0xdf03a353, 0xcf4fcd8f, 0xf12cab16, 0xdc2de6e1},
{0xd071df14, 0x9c17cc1a, 0x63415530, 0xd7c5e6a3, 0x68f3fb1e, 0xb5301660, 0x18269301, 0xb5f70bc9}},
{{0x79ec1a0f, 0x2d8daefd, 0xceb39c97, 0x3bbcd6fd, 0x58f61a95, 0xf5575ffc, 0xadf7b420, 0xdbd986c4},
{0x15f39eb7, 0x81aa8814, 0xb98d976c, 0x6ee2fcf5, 0xcf2f717d, 0x5465475d, 0x6860bbd0, 0x8e24d3c4}}
}
};

// Takes the leftmost 256 bits in hash (treated as big endian),
// and converts to little endian integer z.
static void hash_to_z(uint32_t z[8], const uint8_t* hash, uint32_t hashlen) {
    if (hashlen > 32) {
        hashlen = 32;
    }
    for (uint32_t i = 0; i < hashlen; i++) {
        ((uint8_t*)z)[i] = hash[hashlen - 1 - i];
    }
    for (uint32_t i = hashlen; i < 32; i++) {
        ((uint8_t*)z)[i] = 0;
    }
}

// Creates a representation of a (little endian integer),
// so that r[0] + 2*r[1] + 2^2*r[2] + 2^3*r[3] + ... = a,
// where each r[i] is -15, -13, ..., 11, 13, 15 or 0.
// Only around 1/5.5 of the r[i] will be non-zero.
static void slide_257(signed char r[257], const uint8_t a[32]) {
    for (int i = 0; i < 256; ++i) {
        r[i] = 1 & (a[i >> 3] >> (i & 7));
    }
    r[256] = 0;

    for (int i = 0; i < 256; i++) {
        if (r[i] != 0) {
            for (int b = 1; b <= 4 && i + b < 256; b++) {
                if (r[i + b] != 0) {
                    if (r[i] + (r[i + b] << b) <= 15) {
                        r[i] = (signed char) (r[i] + (r[i + b] << b)); r[i + b] = 0;
                    } else if (r[i] - (r[i + b] << b) >= -15) {
                        r[i] = (signed char) (r[i] - (r[i + b] << b));
                        for (;;) {
                            r[i + b] = 0;
                            b++;
                            if (!r[i + b]) {
                                r[i + b] = 1;
                                b--; // Will be added back after loop footer b++
                                break;
                            }
                        }
                    } else {
                        break;
                    }
                }
            }
        }
    }
}

static void P256_mod_n_inv(uint32_t out[8], const uint32_t in[8]) {
    // This function follows the algorithm in section 12.1 of https://gcd.cr.yp.to/safegcd-20190413.pdf.
    // It has been altered in the following ways:
    //   1. Due to 32-bit cpu, we use 24 * 31 iterations instead of 12 * 62.
    //   2. P-256 modulus instead of 2^255-19.
    //      744 iterations are still enough and slightly more than the required 741 (floor((49*256+57)/17)).
    //   3. Step 5 has been corrected to go back to step 2 instead of step 3.
    //   4. The order of the matrix multiplications in step 6 has been changed to (T24*(T23*(T22*(...*(T1*[0, 1]))))),
    //      where [0, 1] is a column vector to make it possible to be able to extract the "top-right corner", v, of T24*T23*...*T1.
    //      The result v will then be contained in the first element of the resulting column vector.
    
    struct {
        struct FGInteger fg[2]; // f and g
        struct XYInteger xy[2]; // x and y
    } state[2]; // "current" and "next"
    
    state[0].fg[0].flip_sign = 0; // non-negative f
    memcpy(&state[0].fg[0].signed_value, P256_order, 36); // f
    state[0].fg[1].flip_sign = 0; // non-negative g
    memcpy(&state[0].fg[1].signed_value, in, 32); // g
    state[0].fg[1].signed_value[8] = 0; // upper bits of g are 0
    memset(&state[0].xy, 0, sizeof(state[0].xy));
    // We later need a factor 2^-744. The montgomery multiplication gives 2^(24*-32)=2^-768, so multiply the init value (1) by 2^24 here.
    state[0].xy[1].value[0] = 1U << 24;
    
    int delta = 1;
    for (int i = 0; i < 24; i++) {
        // Scaled translation matrix Ti
        uint32_t matrix[4]; // element range: [-2^30, 2^31] (negative numbers are stored in two's complement form)
        
        // Decode f and g into two's complement representation and use the lowest 32 bits in the P256_divsteps2_31 calculation
        uint32_t negate_f = state[i % 2].fg[0].flip_sign;
        uint32_t negate_g = state[i % 2].fg[1].flip_sign;
        delta = P256_divsteps2_31(delta, (state[i % 2].fg[0].signed_value[0] ^ negate_f) - negate_f, (state[i % 2].fg[1].signed_value[0] ^ negate_g) - negate_g, matrix);
        
        // "Jump step", calculates the new f and g values that applies after 31 divstep2 iterations
        P256_matrix_mul_fg_9(matrix[0], matrix[1], state[i % 2].fg, &state[(i + 1) % 2].fg[0]);
        P256_matrix_mul_fg_9(matrix[2], matrix[3], state[i % 2].fg, &state[(i + 1) % 2].fg[1]);
        
        // Iterate the result vector
        // Due to montgomery multiplication inside this function, each step also adds a 2^-32 factor
        P256_matrix_mul_mod_n(matrix[0], matrix[1], state[i % 2].xy, &state[(i + 1) % 2].xy[0]);
        P256_matrix_mul_mod_n(matrix[2], matrix[3], state[i % 2].xy, &state[(i + 1) % 2].xy[1]);
    }
    // Calculates val^-1 = sgn(f) * v * 2^-744, where v is the "top-right corner" of the resulting T24*T23*...*T1 matrix.
    // In this implementation, at this point x contains v * 2^-744.
    P256_negate_mod_n_if(out, &state[0].xy[0].value[0], (state[0].xy[0].flip_sign ^ state[0].fg[0].flip_sign ^ state[0].fg[0].signed_value[8]) & 1);
}

// Constant time abs
static inline uint32_t abs_int(int8_t a) {
    uint32_t a_u = (uint32_t)(int32_t)a;
    uint32_t mask = a_u >> 31;
    mask |= mask << 1;
    mask |= mask << 2;
    uint32_t result = (-a) & mask;
    result |= a & (mask ^ 0xf);
    return result;
}

// Calculates scalar*P in constant time (except for the scalars 2 and n-2, for which the results take a few extra cycles to compute)
static void scalarmult_variable_base(uint32_t output_mont_x[8], uint32_t output_mont_y[8], const uint32_t input_mont_x[8], const uint32_t input_mont_y[8], const uint32_t scalar[8]) {
    // Based on https://eprint.iacr.org/2014/130.pdf, Algorithm 1.
    
    uint32_t scalar2[8];
    int8_t e[64];
    
    // The algorithm used requires the scalar to be odd. If even, negate the scalar modulo p to make it odd, and later negate the end result.
    bool even = (scalar[0] & 1) ^ 1;
    P256_negate_mod_n_if(scalar2, scalar, even);
    
    // Rewrite the scalar as e[0] + 2^4*e[1] + 2^8*e[2] + ... + 2^252*e[63], where each e[i] is an odd number and -15 <= e[i] <= 15.
    e[0] = scalar2[0] & 0xf;
    for (int i = 1; i < 64; i++) {
        // Extract 4 bits
        e[i] = (scalar2[i / 8] >> ((i % 8) * 4)) & 0xf;
        // If even, subtract 2^4 from e[i - 1] and add 1 to e[i]
        e[i - 1] -= (int8_t) (((e[i] & 1) ^ 1) << 4);
        e[i] |= 1;
    }
    
    // Create a table of P, 3P, 5P, ... 15P.
    uint32_t table[8][3][8];
    memcpy(table[0][0], input_mont_x, 32);
    memcpy(table[0][1], input_mont_y, 32);
    memcpy(table[0][2], one_montgomery, 32);
    P256_double_j(table[7], (constarr)table[0]);
    for (int i = 1; i < 8; i++) {
        memcpy(table[i], table[7], 96);
        P256_add_sub_j(table[i], (constarr)table[i - 1], 0, 0);
    }
    
    // Calculate the result as (((((((((e[63]*G)*2^4)+e[62])*2^4)+e[61])*2^4)...)+e[1])*2^4)+e[0] = (2^252*e[63] + 2^248*e[62] + ... + e[0])*G.
    
    uint32_t current_point[3][8];
    
    // e[63] is never negative
    P256_select_point(current_point, (uint32_t*)table, 3, e[63] >> 1);
    
    for (uint32_t i = 63; i --> 0;) {
        for (int j = 3; j >= 0; j--) {
            P256_double_j(current_point, (constarr)current_point);
        }
        uint32_t selected_point[3][8];
        P256_select_point(selected_point, (uint32_t*)table, 3, abs_int(e[i]) >> 1);
        P256_negate_mod_p_if(selected_point[1], selected_point[1], (uint8_t)e[i] >> 7);
        
        // There is (only) one odd input scalar that leads to an exception when i == 0: n-2,
        // in that case current_point will be equal to selected_point and hence a doubling
        // will occur instead. We don't bother fixing the same constant time for that case since
        // the probability of that random value to be generated is around 1/2^255 and an
        // attacker could easily test this case anyway.
        P256_add_sub_j(current_point, (constarr)selected_point, false, false);
    }
    P256_jacobian_to_affine(output_mont_x, output_mont_y, (constarr)current_point);
    
    // If the scalar was initially even, we now negate the result to get the correct result, since -(scalar*G) = (-scalar*G).
    // This is done by negating y, since -(x,y) = (x,-y).
    P256_negate_mod_p_if(output_mont_y, output_mont_y, even);
}

#define get_bit(arr, i) ((arr[(i) / 32] >> ((i) % 32)) & 1)

// Calculates scalar*G in constant time
static void scalarmult_fixed_base(uint32_t output_mont_x[8], uint32_t output_mont_y[8], const uint32_t scalar[8]) {
    uint32_t scalar2[8];
    
    // Just as with the algorithm used in variable base scalar multiplication, this algorithm requires the scalar to be odd.
    bool even = (scalar[0] & 1) ^ 1;
    P256_negate_mod_n_if(scalar2, scalar, even);
    
    // This algorithm conceptually rewrites the odd scalar as s[0] + 2^1*s[1] + 2^2*s[2] + ... + 2^255*s[255], where each s[i] is -1 or 1.
    // By initially setting s[i] to the corresponding bit S[i] in the original odd scalar S, we go from lsb to msb, and whenever a value s[i] is 0,
    // increase s[i] by 1 and decrease s[i-1] by 2.
    // This will result in that s[i] = S[i+1] == 1 ? 1 : -1 for i < 255, and s[255] = 1.
    
    // We then form the scalars abs(s[j] + s[j+64]*2^64 + s[j+128]*2^128 + s[j+192]*2^192)*(2^32 * floor(j / 32)) for different 0 <= j < 64.
    // Each scalar times G has already been precomputed in p256_basepoint_precomp2.
    // That way we only need 31 point doublings and 63 point additions.
    
    uint32_t current_point[3][8];
    uint32_t selected_point[2][8];
    
    for (uint32_t i = 32; i --> 0;) {
        {
            uint32_t mask = get_bit(scalar2, i + 32 + 1) | (get_bit(scalar2, i + 64 + 32 + 1) << 1) | (get_bit(scalar2, i + 2 * 64 + 32 + 1) << 2);
            if (i == 31) {
                P256_select_point(current_point, (uint32_t*)p256_basepoint_precomp2[1], 2, mask);
                memcpy(current_point[2], one_montgomery, 32);
            } else {
                P256_double_j(current_point, (constarr)current_point);
                
                uint32_t sign = get_bit(scalar2, i + 3 * 64 + 32 + 1) - 1; // positive: 0, negative: -1
                mask = (mask ^ sign) & 7;
                P256_select_point(selected_point, (uint32_t*)p256_basepoint_precomp2[1], 2, mask);
                P256_negate_mod_p_if(selected_point[1], selected_point[1], sign & 1);
                P256_add_sub_j(current_point, (constarr)selected_point, false, true);
            }
        }
        {
            uint32_t mask = get_bit(scalar2, i + 1) | (get_bit(scalar2, i + 64 + 1) << 1) | (get_bit(scalar2, i + 2 * 64 + 1) << 2);
            uint32_t sign = get_bit(scalar2, i + 3 * 64 + 1) - 1; // positive: 0, negative: -1
            mask = (mask ^ sign) & 7;
            P256_select_point(selected_point, (uint32_t*)p256_basepoint_precomp2[0], 2, mask);
            P256_negate_mod_p_if(selected_point[1], selected_point[1], sign & 1);
            P256_add_sub_j(current_point, (constarr)selected_point, false, true);
        }
    }
    P256_jacobian_to_affine(output_mont_x, output_mont_y, (constarr)current_point);
    
    // Negate final result if the scalar was initially even.
    P256_negate_mod_p_if(output_mont_y, output_mont_y, even);
}

static void p256_convert_endianness(void* output, const void* input, size_t byte_len) {
    for (size_t i = 0; i < byte_len / 2; i++) {
        uint8_t t = ((uint8_t*)input)[byte_len - 1 - i];
        ((uint8_t*)output)[byte_len - 1 - i] = ((uint8_t*)input)[i];
        ((uint8_t*)output)[i] = t;
    }
}

static bool p256_verify(const uint32_t public_key_x[8], const uint32_t public_key_y[8], const uint8_t* hash, uint32_t hashlen_in_bytes, const uint32_t r[8], const uint32_t s[8]) {
    if (!P256_check_range_n(r) || !P256_check_range_n(s)) {
        return false;
    }
    
    if (!P256_check_range_p(public_key_x) || !P256_check_range_p(public_key_y)) {
        return false;
    }
    
    uint32_t pk_table[8][3][8];
    P256_to_montgomery(pk_table[0][0], public_key_x);
    P256_to_montgomery(pk_table[0][1], public_key_y);
    memcpy(pk_table[0][2], one_montgomery, 32);
    
    if (!P256_point_is_on_curve(pk_table[0][0], pk_table[0][1])) {
        return false;
    }
    
    // Create a table of P, 3P, 5P, ..., 15P, where P is the public key.
    P256_double_j(pk_table[7], (constarr)pk_table[0]);
    for (int i = 1; i < 8; i++) {
        memcpy(pk_table[i], pk_table[7], 96);
        P256_add_sub_j(pk_table[i], (constarr)pk_table[i - 1], 0, 0);
    }
    
    uint32_t z[8], w[8], u1[8], u2[8];
    
    hash_to_z(z, hash, hashlen_in_bytes);
    
    P256_mod_n_inv(w, s);
    
    P256_mul_mod_n(u1, z, w);
    P256_mul_mod_n(u2, r, w);
    
    // Each value in these arrays will be an odd integer v, so that -15 <= v <= 15.
    // Around 1/5.5 of them will be non-zero.
    signed char slide_bp[257], slide_pk[257];
    slide_257(slide_bp, (uint8_t*)u1);
    slide_257(slide_pk, (uint8_t*)u2);
    
    uint32_t cp[3][8] = {0};
    
    for (int i = 256; i >= 0; i--) {
        P256_double_j(cp, (constarr)cp);
        if (slide_bp[i] > 0) {
            P256_add_sub_j(cp, p256_basepoint_precomp[slide_bp[i]/2], 0, 1);
        } else if (slide_bp[i] < 0) {
            P256_add_sub_j(cp, p256_basepoint_precomp[(-slide_bp[i])/2], 1, 1);
        }
        if (slide_pk[i] > 0) {
            P256_add_sub_j(cp, (constarr)pk_table[slide_pk[i]/2], 0, 0);
        } else if (slide_pk[i] < 0) {
            P256_add_sub_j(cp, (constarr)pk_table[(-slide_pk[i])/2], 1, 0);
        }
    }
    
    return P256_verify_last_step(r, (constarr)cp);
}

static bool p256_sign_step1(struct SignPrecomp *result, const uint32_t k[8]) {
    do {
        uint32_t point_res[2][8];
        if (!P256_check_range_n(k)) {
            break;
        }
        scalarmult_fixed_base(point_res[0], point_res[1], k);
        P256_mod_n_inv(result->k_inv, k);
        P256_from_montgomery(result->r, point_res[0]);
        P256_reduce_mod_n_32bytes(result->r, result->r);
        
        uint32_t r_sum = 0;
        for (int i = 0; i < 8; i++) {
            r_sum |= result->r[i];
        }
        if (r_sum == 0) {
            break;
        }
        return true;
    } while (false);
    
    memset(result, 0, sizeof(struct SignPrecomp));
    return false;
}

static bool p256_sign_step2(uint32_t r[8], uint32_t s[8], const uint8_t* hash, uint32_t hashlen_in_bytes, const uint32_t private_key[8], struct SignPrecomp *sign_precomp) {
    do {
        if (!P256_check_range_n(sign_precomp->k_inv) || !P256_check_range_n(sign_precomp->r)) { // just make sure user did not input an obviously invalid precomp
            break;
        }
        uint32_t *const z = r;
        hash_to_z(z, hash, hashlen_in_bytes);
        P256_mul_mod_n(s, sign_precomp->r, private_key);
        P256_add_mod_n(s, z, s);
        P256_mul_mod_n(s, sign_precomp->k_inv, s);
        
        memcpy(r, sign_precomp->r, 32);
        
        uint32_t s_sum = 0;
        for (int i = 0; i < 8; i++) {
            s_sum |= s[i];
        }
        if (s_sum == 0) {
            break;
        }
        memset(sign_precomp, 0, sizeof(*sign_precomp));
        return true;
    } while (false);
    
    memset(r, 0, 32);
    memset(s, 0, 32);
    return false;
}

static bool p256_sign(uint32_t r[8], uint32_t s[8], const uint8_t* hash, uint32_t hashlen_in_bytes, const uint32_t private_key[8], const uint32_t k[8]) {
    struct SignPrecomp t;
    if (!p256_sign_step1(&t, k)) {
        memset(r, 0, 32);
        memset(s, 0, 32);
        return false;
    }
    return p256_sign_step2(r, s, hash, hashlen_in_bytes, private_key, &t);
}

static bool p256_scalarmult_base(uint32_t result_x[8], uint32_t result_y[8], const uint32_t scalar[8]) {
    if (!P256_check_range_n(scalar)) {
        return false;
    }
    scalarmult_fixed_base(result_x, result_y, scalar);
    P256_from_montgomery(result_x, result_x);
    P256_from_montgomery(result_y, result_y);
    return true;
    
}

static bool p256_keygen(uint32_t public_key_x[8], uint32_t public_key_y[8], const uint32_t private_key[8]) {
    return p256_scalarmult_base(public_key_x, public_key_y, private_key);
}


static bool p256_scalarmult_generic_no_scalar_check(uint32_t output_mont_x[8], uint32_t output_mont_y[8], const uint32_t scalar[8], const uint32_t in_x[8], const uint32_t in_y[8]) {
    if (!P256_check_range_p(in_x) || !P256_check_range_p(in_y)) {
        return false;
    }
    
    P256_to_montgomery(output_mont_x, in_x);
    P256_to_montgomery(output_mont_y, in_y);
    
    if (!P256_point_is_on_curve(output_mont_x, output_mont_y)) {
        return false;
    }
    
    scalarmult_variable_base(output_mont_x, output_mont_y, output_mont_x, output_mont_y, scalar);
    return true;
}

static bool p256_ecdh_calc_shared_secret(uint8_t shared_secret[32], const uint32_t private_key[8], const uint32_t others_public_key_x[8], const uint32_t others_public_key_y[8]) {
    uint32_t result_x[8], result_y[8];
    if (!p256_scalarmult_generic_no_scalar_check(result_x, result_y, private_key, others_public_key_x, others_public_key_y)) {
        return false;
    }
    P256_from_montgomery(result_x, result_x);
    p256_convert_endianness(shared_secret, result_x, 32);
    return true;
}

// Assembler, from p256-cortex-m4-asm-gcc.S, converted to a top-level asm
// block so it can live in the amalgamated mongoose.c
__asm__(
    ".pushsection .text.mg_p256,\"ax\",%progbits\n"
    "\t.syntax unified\n"
    "\t.thumb\n"
    "\t.align 2\n"
    "\t.type P256_select_point, %function\n"
    "P256_select_point:\n"
    "\t.global P256_select_point\n"
    "\tpush {r0,r2,r3,r4-r11,lr}\n"
    "\tsubs r2,#1\n"
    "\tlsls r2,#5\n"
    "0:\n"
    "\trsbs r3,#0\n"
    "\tsbcs r3,r3,r3\n"
    "\tmvns r3,r3\n"
    "\tldm r1!,{r6-r12,lr}\n"
    "\tands r6,r3\n"
    "\tands r7,r3\n"
    "\tand r8,r3\n"
    "\tand r9,r3\n"
    "\tand r10,r3\n"
    "\tand r11,r3\n"
    "\tand r12,r3\n"
    "\tand lr,r3\n"
    "\tadds r1,r2\n"
    "\tmovs r3,#1\n"
    "1:\n"
    "\tldr r0,[sp,#8]\n"
    "\teors r0,r3\n"
    "\tmrs r0,apsr\n"
    "\tlsrs r0,#30\n"
    "\tldm r1!,{r4,r5}\n"
    "\tumlal r6,r2,r0,r4\n"
    "\tumlal r7,r3,r0,r5\n"
    "\tldm r1!,{r4,r5}\n"
    "\tumlal r8,r2,r0,r4\n"
    "\tumlal r9,r3,r0,r5\n"
    "\tldm r1!,{r4,r5}\n"
    "\tumlal r10,r2,r0,r4\n"
    "\tumlal r11,r3,r0,r5\n"
    "\tldm r1!,{r4,r5}\n"
    "\tumlal r12,r2,r0,r4\n"
    "\tumlal lr,r3,r0,r5\n"
    "\tadds r1,r2\n"
    "\tadds r3,#1\n"
    "\tcmp r3,#8\n"
    "\tbne 1b\n"
    "\tldm sp,{r0,r4}\n"
    "\tstm r0!,{r6-r12,lr}\n"
    "\tstr r0,[sp]\n"
    "\tsub r1,r1,r2, lsl #3\n"
    "\tsubs r1,#224\n"
    "\tsubs r4,#1\n"
    "\tstr r4,[sp,#4]\n"
    "\tldr r3,[sp,#8]\n"
    "\tbne 0b\n"
    "\tadd sp,#12\n"
    "\tpop {r4-r11,pc}\n"
    "\t.size P256_select_point, .-P256_select_point\n"
    "\t.type mul288x288, %function\n"
    "mul288x288:\n"
    "\tpush {r4-r11,lr}\n"
    "\tmov r4,r0\n"
    "\tmov r5,r2\n"
    "\tmov r6,r1\n"
    "\tmovs r1,#72\n"
    "\tbl setzero\n"
    "\tldm r5,{r0-r2,r8-r12,lr}\n"
    "\tmovs r7,#9\n"
    "0:\n"
    "\tldm r6!,{r5}\n"
    "\tpush {r6,r7}\n"
    "\tmovs r3,#0\n"
    "\tldm r4,{r6,r7}\n"
    "\tumaal r6,r3,r5,r0\n"
    "\tumaal r7,r3,r5,r1\n"
    "\tstm r4!,{r6,r7}\n"
    "\tldm r4,{r6,r7}\n"
    "\tumaal r6,r3,r5,r2\n"
    "\tumaal r7,r3,r5,r8\n"
    "\tstm r4!,{r6,r7}\n"
    "\tldm r4,{r6,r7}\n"
    "\tumaal r6,r3,r5,r9\n"
    "\tumaal r7,r3,r5,r10\n"
    "\tstm r4!,{r6,r7}\n"
    "\tldm r4,{r6,r7}\n"
    "\tumaal r6,r3,r5,r11\n"
    "\tumaal r7,r3,r5,r12\n"
    "\tstm r4!,{r6,r7}\n"
    "\tldm r4,{r6}\n"
    "\tumaal r3,r6,r5,lr\n"
    "\tstm r4!,{r3,r6}\n"
    "\tsubs r4,r4,#36\n"
    "\tpop {r6,r7}\n"
    "\tsubs r7,r7,#1\n"
    "\tbne 0b\n"
    "\tpop {r4-r11,pc}\n"
    "\t.size mul288x288, .-mul288x288\n"
    "\t.type setzero, %function\n"
    "setzero:\n"
    "\tmovs r2,#0\n"
    "\tmovs r3,#0\n"
    "0:\n"
    "\tstm r0!,{r2,r3}\n"
    "\tsubs r1,r1,#8\n"
    "\tbne 0b\n"
    "\tbx lr\n"
    "\t.size setzero, .-setzero\n"
    "\t.type P256_mulmod, %function\n"
    "P256_mulmod:\n"
    "\tpush {r2,lr}\n"
    "\tsub sp,#28\n"
    "\tldm r2,{r2,r3,r4,r5}\n"
    "\tldm r1!,{r0,r10,lr}\n"
    "\tumull r6,r11,r2,r0\n"
    "\tumull r7,r12,r3,r0\n"
    "\tumaal r7,r11,r2,r10\n"
    "\tpush {r6,r7}\n"
    "\tumull r8,r6,r4,r0\n"
    "\tumaal r8,r11,r3,r10\n"
    "\tumull r9,r7,r5,r0\n"
    "\tumaal r9,r11,r4,r10\n"
    "\tumaal r11,r7,r5,r10\n"
    "\tumaal r8,r12,r2,lr\n"
    "\tumaal r9,r12,r3,lr\n"
    "\tumaal r11,r12,r4,lr\n"
    "\tumaal r12,r7,r5,lr\n"
    "\tldm r1!,{r0,r10,lr}\n"
    "\tumaal r9,r6,r2,r0\n"
    "\tumaal r11,r6,r3,r0\n"
    "\tumaal r12,r6,r4,r0\n"
    "\tumaal r6,r7,r5,r0\n"
    "\tstrd r8,r9,[sp,#8]\n"
    "\tmov r9,#0\n"
    "\tumaal r11,r9,r2,r10\n"
    "\tumaal r12,r9,r3,r10\n"
    "\tumaal r6,r9,r4,r10\n"
    "\tumaal r7,r9,r5,r10\n"
    "\tmov r10,#0\n"
    "\tumaal r12,r10,r2,lr\n"
    "\tumaal r6,r10,r3,lr\n"
    "\tumaal r7,r10,r4,lr\n"
    "\tumaal r9,r10,r5,lr\n"
    "\tldr r8,[r1],#4\n"
    "\tmov lr,#0\n"
    "\tumaal lr,r6,r2,r8\n"
    "\tumaal r7,r6,r3,r8\n"
    "\tumaal r9,r6,r4,r8\n"
    "\tumaal r10,r6,r5,r8\n"
    "\tldr r8,[r1],#-28\n"
    "\tmov r0,#0\n"
    "\tumaal r7,r0,r2,r8\n"
    "\tumaal r9,r0,r3,r8\n"
    "\tumaal r10,r0,r4,r8\n"
    "\tumaal r6,r0,r5,r8\n"
    "\tpush {r0}\n"
    "\tldr r2,[sp,#40]\n"
    "\tadds r2,r2,#16\n"
    "\tldm r2,{r2,r3,r4,r5}\n"
    "\tldr r8,[r1],#4\n"
    "\tmov r0,#0\n"
    "\tumaal r11,r0,r2,r8\n"
    "\tstr r11,[sp,#16+4]\n"
    "\tumaal r12,r0,r3,r8\n"
    "\tumaal lr,r0,r4,r8\n"
    "\tumaal r0,r7,r5,r8\n"
    "\tldr r8,[r1],#4\n"
    "\tmov r11,#0\n"
    "\tumaal r12,r11,r2,r8\n"
    "\tstr r12,[sp,#20+4]\n"
    "\tumaal lr,r11,r3,r8\n"
    "\tumaal r0,r11,r4,r8\n"
    "\tumaal r11,r7,r5,r8\n"
    "\tldr r8,[r1],#4\n"
    "\tmov r12,#0\n"
    "\tumaal lr,r12,r2,r8\n"
    "\tstr lr,[sp,#24+4]\n"
    "\tumaal r0,r12,r3,r8\n"
    "\tumaal r11,r12,r4,r8\n"
    "\tumaal r10,r12,r5,r8\n"
    "\tldr r8,[r1],#4\n"
    "\tmov lr,#0\n"
    "\tumaal r0,lr,r2,r8\n"
    "\tstr r0,[sp,#28+4]\n"
    "\tumaal r11,lr,r3,r8\n"
    "\tumaal r10,lr,r4,r8\n"
    "\tumaal r6,lr,r5,r8\n"
    "\tldm r1!,{r0,r8}\n"
    "\tumaal r11,r9,r2,r0\n"
    "\tstr r11,[sp,#32+4]\n"
    "\tumaal r9,r10,r3,r0\n"
    "\tumaal r10,r6,r4,r0\n"
    "\tpop {r11}\n"
    "\tumaal r11,r6,r5,r0\n"
    "\tumaal r9,r7,r2,r8\n"
    "\tumaal r10,r7,r3,r8\n"
    "\tumaal r11,r7,r4,r8\n"
    "\tumaal r6,r7,r5,r8\n"
    "\tldm r1!,{r0,r8}\n"
    "\tumaal r10,r12,r2,r0\n"
    "\tumaal r11,r12,r3,r0\n"
    "\tumaal r6,r12,r4,r0\n"
    "\tumaal r7,r12,r5,r0\n"
    "\tumaal r11,lr,r2,r8\n"
    "\tumaal lr,r6,r3,r8\n"
    "\tumaal r6,r7,r4,r8\n"
    "\tumaal r7,r12,r5,r8\n"
    "\tpush {r6,r7,r12}\n"
    "\tadd r7,sp,#12\n"
    "\tldm r7,{r0-r8}\n"
    "\tmov r12,#0\n"
    "\tadds r3,r0\n"
    "\tadcs r4,r4,r1\n"
    "\tadcs r5,r5,r2\n"
    "\tadcs r6,r6,r0\n"
    "\tadcs r7,r7,r1\n"
    "\tadcs r8,r8,r0\n"
    "\tadcs r9,r9,r1\n"
    "\tadcs r10,r10,#0\n"
    "\tadcs r11,r11,#0\n"
    "\tadcs r12,r12,#0\n"
    "\tadds r6,r3\n"
    "\tadcs r7,r7,r4\n"
    "\tadcs r8,r8,r2\n"
    "\tadcs r9,r9,r3\n"
    "\tadcs r10,r10,r2\n"
    "\tadcs r11,r11,r3\n"
    "\tadcs r12,r12,#0\n"
    "\tsubs r7,r0\n"
    "\tsbcs r8,r8,r1\n"
    "\tsbcs r9,r9,r2\n"
    "\tsbcs r10,r10,r3\n"
    "\tsbcs r11,r11,#0\n"
    "\tsbcs r12,r12,#0\n"
    "\tpop {r1-r3}\n"
    "\tadds r0,lr,r12\n"
    "\tadcs r1,r1,#0\n"
    "\tmov r12,#0\n"
    "\tadcs r12,r12,#0\n"
    "\tadcs r8,r8,r5\n"
    "\tadcs r9,r9,r6\n"
    "\tadcs r10,r10,r4\n"
    "\tadcs r11,r11,r5\n"
    "\tadcs r0,r0,r4\n"
    "\tadcs r1,r1,r5\n"
    "\tadcs r2,r2,r12\n"
    "\tadcs r3,r3,#0\n"
    "\tmov r12,#0\n"
    "\tadcs r12,r12,#0\n"
    "\tadcs r10,r10,r7\n"
    "\tadcs r11,r11,#0\n"
    "\tadcs r0,r0,r6\n"
    "\tadcs r1,r1,r7\n"
    "\tadcs r2,r2,r6\n"
    "\tadcs r3,r3,r7\n"
    "\tadcs r12,r12,#0\n"
    "\tsubs r11,r4\n"
    "\tsbcs r0,r0,r5\n"
    "\tsbcs r1,r1,r6\n"
    "\tsbcs r2,r2,r7\n"
    "\tsbcs r3,r3,#0\n"
    "\tsbcs r12,r12,#0\n"
    "\tsubs r8,r8,#0xffffffff\n"
    "\tsbcs r9,r9,#0xffffffff\n"
    "\tsbcs r10,r10,#0xffffffff\n"
    "\tsbcs r11,r11,#0\n"
    "\tsbcs r4,r0,#0\n"
    "\tsbcs r5,r1,#0\n"
    "\tsbcs r6,r2,#1\n"
    "\tsbcs r7,r3,#0xffffffff\n"
    "\tsbc r12,r12,#0\n"
    "\tadds r0,r8,r12\n"
    "\tadcs r1,r9,r12\n"
    "\tadcs r2,r10,r12\n"
    "\tadcs r3,r11,#0\n"
    "\tadcs r4,r4,#0\n"
    "\tadcs r5,r5,#0\n"
    "\tadcs r6,r6,r12, lsr #31\n"
    "\tadcs r7,r7,r12\n"
    "\tadd sp,sp,#40\n"
    "\tpop {pc}\n"
    "\t.size P256_mulmod, .-P256_mulmod\n"
    "\t.type P256_sqrmod, %function\n"
    "P256_sqrmod:\n"
    "\tpush {lr}\n"
    "\tumull r9,r10,r0,r0\n"
    "\tumull r11,r12,r0,r1\n"
    "\tadds r11,r11,r11\n"
    "\tmov lr,#0\n"
    "\tumaal r10,r11,lr,lr\n"
    "\tpush {r9,r10}\n"
    "\tmov r9,#0\n"
    "\tumaal r9,r12,r0,r2\n"
    "\tadcs r9,r9,r9\n"
    "\tumaal r9,r11,r1,r1\n"
    "\tpush {r9}\n"
    "\tumull r9,r10,r0,r3\n"
    "\tumaal r9,r12,r1,r2\n"
    "\tadcs r9,r9,r9\n"
    "\tumaal r9,r11,lr,lr\n"
    "\tpush {r9}\n"
    "\tmov r9,#0\n"
    "\tumaal r9,r10,r0,r4\n"
    "\tumaal r9,r12,r1,r3\n"
    "\tadcs r9,r9,r9\n"
    "\tumaal r9,r11,r2,r2\n"
    "\tpush {r9}\n"
    "\tumull r9,r8,r0,r5\n"
    "\tumaal r9,r10,r1,r4\n"
    "\tumaal r9,r12,r2,r3\n"
    "\tadcs r9,r9,r9\n"
    "\tumaal r9,r11,lr,lr\n"
    "\tpush {r9}\n"
    "\tmov r9,#0\n"
    "\tumaal r9,r8,r1,r5\n"
    "\tumaal r9,r12,r2,r4\n"
    "\tumaal r9,r10,r0,r6\n"
    "\tadcs r9,r9,r9\n"
    "\tumaal r9,r11,r3,r3\n"
    "\tpush {r9}\n"
    "\tumull r9,r0,r0,r7\n"
    "\tumaal r9,r10,r1,r6\n"
    "\tumaal r9,r12,r2,r5\n"
    "\tumaal r9,r8,r3,r4\n"
    "\tadcs r9,r9,r9\n"
    "\tumaal r9,r11,lr,lr\n"
    "\tumaal r0,r8,r1,r7\n"
    "\tumaal r0,r10,r2,r6\n"
    "\tumaal r0,r12,r3,r5\n"
    "\tadcs r0,r0,r0\n"
    "\tumaal r11,r0,r4,r4\n"
    "\tumaal r12,r8,r2,r7\n"
    "\tumaal r12,r10,r3,r6\n"
    "\tmovs r2,#0\n"
    "\tumaal r12,r2,r4,r5\n"
    "\tadcs r1,r12,r12\n"
    "\tumaal r0,r1,lr,lr\n"
    "\tumaal r2,r8,r3,r7\n"
    "\tumaal r2,r10,r4,r6\n"
    "\tadcs r2,r2,r2\n"
    "\tumaal r1,r2,r5,r5\n"
    "\tmovs r3,#0\n"
    "\tumaal r3,r8,r4,r7\n"
    "\tumaal r3,r10,r5,r6\n"
    "\tadcs r3,r3,r3\n"
    "\tumaal r2,r3,lr,lr\n"
    "\tumaal r8,r10,r5,r7\n"
    "\tadcs r8,r8,r8\n"
    "\tumaal r3,r8,r6,r6\n"
    "\tumull r4,r5,lr,lr\n"
    "\tumaal r4,r10,r6,r7\n"
    "\tadcs r4,r4,r4\n"
    "\tumaal r4,r8,lr,lr\n"
    "\tadcs r10,r10,r10\n"
    "\tumaal r8,r10,r7,r7\n"
    "\tadcs r10,r10,lr\n"
    "\tpush {r4,r8,r10}\n"
    "\tadd r4,sp,#12\n"
    "\tldm r4,{r4-r8,r10,r12}\n"
    "\tX0 .req r10\n"
    "\tX1 .req r12\n"
    "\tX2 .req r8\n"
    "\tX3 .req r7\n"
    "\tX4 .req r6\n"
    "\tX5 .req r5\n"
    "\tX6 .req r4\n"
    "\tX7 .req r9\n"
    "\tX8 .req r11\n"
    "\tX9 .req r0\n"
    "\tX10 .req r1\n"
    "\tX11 .req r2\n"
    "\tX12 .req r3\n"
    "\tX13 .req r7\n"
    "\tX14 .req r8\n"
    "\tX15 .req r12\n"
    "\tadcs X3,X3,X0\n"
    "\tadcs X4,X4,X1\n"
    "\tadcs X5,X5,X2\n"
    "\tadcs X6,X6,X0\n"
    "\tadcs X7,X7,X1\n"
    "\tadcs X8,X8,X0\n"
    "\tadcs X9,X9,X1\n"
    "\tadcs X10,X10,#0\n"
    "\tadcs X11,X11,#0\n"
    "\tadcs lr,lr,#0\n"
    "\tadds X6,X3\n"
    "\tadcs X7,X7,X4\n"
    "\tadcs X8,X8,X2\n"
    "\tadcs X9,X9,X3\n"
    "\tadcs X10,X10,X2\n"
    "\tadcs X11,X11,X3\n"
    "\tadcs lr,lr,#0\n"
    "\tsubs X7,X0\n"
    "\tsbcs X8,X8,X1\n"
    "\tsbcs X9,X9,X2\n"
    "\tsbcs X10,X10,X3\n"
    "\tsbcs X11,X11,#0\n"
    "\tsbcs lr,lr,#0\n"
    "\tpop {X13,X14,X15}\n"
    "\tadds X0,X12,lr\n"
    "\tadcs X13,X13,#0\n"
    "\tmov lr,#0\n"
    "\tadcs lr,lr,#0\n"
    "\tadcs X8,X8,X5\n"
    "\tadcs X9,X9,X6\n"
    "\tadcs X10,X10,X4\n"
    "\tadcs X11,X11,X5\n"
    "\tadcs X0,X0,X4\n"
    "\tadcs X13,X13,X5\n"
    "\tadcs X14,X14,lr\n"
    "\tadcs X15,X15,#0\n"
    "\tmov lr,#0\n"
    "\tadcs lr,lr,#0\n"
    "\tadcs X10,X10,X7\n"
    "\tadcs X11,X11,#0\n"
    "\tadcs X0,X0,X6\n"
    "\tadcs X13,X13,X7\n"
    "\tadcs X14,X14,X6\n"
    "\tadcs X15,X15,X7\n"
    "\tadcs lr,lr,#0\n"
    "\tsubs X11,X4\n"
    "\tsbcs X0,X0,X5\n"
    "\tsbcs X13,X13,X6\n"
    "\tsbcs X14,X14,X7\n"
    "\tsbcs X15,X15,#0\n"
    "\tsbcs lr,lr,#0\n"
    "\tsubs r11,r11,#0xffffffff\n"
    "\tsbcs r9,r0,#0xffffffff\n"
    "\tsbcs r4,r1,#0xffffffff\n"
    "\tsbcs r3,r2,#0\n"
    "\tsbcs r6,r10,#0\n"
    "\tsbcs r5,r7,#0\n"
    "\tsbcs r10,r8,#1\n"
    "\tsbcs r8,r12,#0xffffffff\n"
    "\tsbcs r7,lr,#0\n"
    "\tadds r0,r11,r7\n"
    "\tadcs r1,r9,r7\n"
    "\tadcs r2,r4,r7\n"
    "\tadcs r3,r3,#0\n"
    "\tadcs r4,r6,#0\n"
    "\tadcs r5,r5,#0\n"
    "\tadcs r6,r10,r7, lsr #31\n"
    "\tadcs r7,r8,r7\n"
    "\tadd sp,#28\n"
    "\tpop {pc}\n"
    "\t.size P256_sqrmod, .-P256_sqrmod\n"
    "\t.type P256_submod, %function\n"
    "P256_submod:\n"
    "\tldm r1,{r3-r10}\n"
    "\tldm r2!,{r0,r1,r11,r12}\n"
    "\tsubs r3,r0\n"
    "\tsbcs r4,r4,r1\n"
    "\tsbcs r5,r5,r11\n"
    "\tsbcs r6,r6,r12\n"
    "\tldm r2,{r0,r1,r11,r12}\n"
    "\tsbcs r7,r7,r0\n"
    "\tsbcs r8,r8,r1\n"
    "\tsbcs r9,r9,r11\n"
    "\tsbcs r10,r10,r12\n"
    "\tsbcs r11,r11,r11\n"
    "\tadds r0,r3,r11\n"
    "\tadcs r1,r4,r11\n"
    "\tadcs r2,r5,r11\n"
    "\tadcs r3,r6,#0\n"
    "\tadcs r4,r7,#0\n"
    "\tadcs r5,r8,#0\n"
    "\tadcs r6,r9,r11, lsr #31\n"
    "\tadcs r7,r10,r11\n"
    "\tbx lr\n"
    "\t.size P256_submod, .-P256_submod\n"
    "\t.type P256_addmod, %function\n"
    "P256_addmod:\n"
    "\tldm r2,{r2-r9}\n"
    "\tldm r1!,{r0,r10,r11,r12}\n"
    "\tadds r2,r0\n"
    "\tadcs r3,r3,r10\n"
    "\tadcs r4,r4,r11\n"
    "\tadcs r5,r5,r12\n"
    "\tldm r1,{r0,r1,r11,r12}\n"
    "\tadcs r6,r6,r0\n"
    "\tadcs r7,r7,r1\n"
    "\tadcs r8,r8,r11\n"
    "\tadcs r9,r9,r12\n"
    "\tmovs r10,#0\n"
    "\tadcs r10,r10,r10\n"
    "\tsubs r2,#0xffffffff\n"
    "\tsbcs r3,r3,#0xffffffff\n"
    "\tsbcs r4,r4,#0xffffffff\n"
    "\tsbcs r5,r5,#0\n"
    "\tsbcs r6,r6,#0\n"
    "\tsbcs r7,r7,#0\n"
    "\tsbcs r8,r8,#1\n"
    "\tsbcs r9,r9,#0xffffffff\n"
    "\tsbcs r10,r10,#0\n"
    "\tadds r0,r2,r10\n"
    "\tadcs r1,r3,r10\n"
    "\tadcs r2,r4,r10\n"
    "\tadcs r3,r5,#0\n"
    "\tadcs r4,r6,#0\n"
    "\tadcs r5,r7,#0\n"
    "\tadcs r6,r8,r10, lsr #31\n"
    "\tadcs r7,r9,r10\n"
    "\tbx lr\n"
    "\t.size P256_addmod, .-P256_addmod\n"
    "\t.type P256_sqrmod_many, %function\n"
    "P256_sqrmod_many:\n"
    "\tpush {r8,lr}\n"
    "0:\n"
    "\tbl P256_sqrmod\n"
    "\tldr r8,[sp,#0]\n"
    "\tsubs r8,r8,#1\n"
    "\tstr r8,[sp,#0]\n"
    "\tbne 0b\n"
    "\tpop {r8,pc}\n"
    "\t.size P256_sqrmod_many, .-P256_sqrmod_many\n"
    "\t.type P256_sqrmod_many_and_mulmod, %function\n"
    "P256_sqrmod_many_and_mulmod:\n"
    "\tpush {r9,lr}\n"
    "\tbl P256_sqrmod_many\n"
    "\tpush {r0-r7}\n"
    "\tmov r1,sp\n"
    "\tldr r2,[sp,#32]\n"
    "\tbl P256_mulmod\n"
    "\tadd sp,#36\n"
    "\tpop {pc}\n"
    "\t.size P256_sqrmod_many_and_mulmod, .-P256_sqrmod_many_and_mulmod\n"
    "\t.type P256_modinv_sqrt, %function\n"
    "P256_modinv_sqrt:\n"
    "\tpush {r0-r8,lr}\n"
    "\tmov r8,#1\n"
    "\tmov r9,sp\n"
    "\tbl P256_sqrmod_many_and_mulmod\n"
    "\tpush {r0-r7}\n"
    "\tbl P256_sqrmod\n"
    "\tbl P256_sqrmod\n"
    "\tpush {r0-r7}\n"
    "\tmov r1,sp\n"
    "\tadd r2,sp,#32\n"
    "\tbl P256_mulmod\n"
    "\tadd r8,sp,#32\n"
    "\tstm r8,{r0-r7}\n"
    "\tmov r8,#8-4\n"
    "\tadd r9,sp,#32\n"
    "\tbl P256_sqrmod_many_and_mulmod\n"
    "\tpush {r0-r7}\n"
    "\tmov r8,#16-8\n"
    "\tmov r9,sp\n"
    "\tbl P256_sqrmod_many_and_mulmod\n"
    "\tpush {r0-r7}\n"
    "\tmov r8,#16\n"
    "\tmov r9,sp\n"
    "\tbl P256_sqrmod_many_and_mulmod\n"
    "\tpush {r0-r7}\n"
    "\tmov r8,#32\n"
    "\tadd r9,sp,#5*32\n"
    "\tbl P256_sqrmod_many_and_mulmod\n"
    "\tldr r8,[sp,#6*32]\n"
    "\tcmp r8,#0\n"
    "\tbne 0f\n"
    "\tmov r8,#192-64\n"
    "\tmov r9,sp\n"
    "\tbl P256_sqrmod_many_and_mulmod\n"
    "\tmov r8,#224-192\n"
    "\tmov r9,sp\n"
    "\tbl P256_sqrmod_many_and_mulmod\n"
    "\tmov r8,#240-224\n"
    "\tadd r9,sp,#32\n"
    "\tbl P256_sqrmod_many_and_mulmod\n"
    "\tmov r8,#248-240\n"
    "\tadd r9,sp,#64\n"
    "\tbl P256_sqrmod_many_and_mulmod\n"
    "\tmov r8,#252-248\n"
    "\tadd r9,sp,#128\n"
    "\tbl P256_sqrmod_many_and_mulmod\n"
    "\tmov r8,#256-252\n"
    "\tadd r9,sp,#96\n"
    "\tbl P256_sqrmod_many_and_mulmod\n"
    "\tstm sp,{r0-r7}\n"
    "\tmov r1,sp\n"
    "\tadd r2,sp,#5*32\n"
    "\tbl P256_mulmod\n"
    "\tb 1f\n"
    "0:\n"
    "\tmov r8,#160-64\n"
    "\tadd r9,sp,#5*32\n"
    "\tbl P256_sqrmod_many_and_mulmod\n"
    "\tmov r8,#254-160\n"
    "\tbl P256_sqrmod_many\n"
    "1:\n"
    "\tadd sp,#6*32+4\n"
    "\tpop {pc}\n"
    "\t.size P256_modinv_sqrt, .-P256_modinv_sqrt\n"
    "\t.type P256_times2, %function\n"
    "P256_times2:\n"
    "\tadds r0,r0\n"
    "\tadcs r1,r1,r1\n"
    "\tadcs r2,r2,r2\n"
    "\tadcs r3,r3,r3\n"
    "\tadcs r4,r4,r4\n"
    "\tadcs r5,r5,r5\n"
    "\tadcs r6,r6,r6\n"
    "\tadcs r7,r7,r7\n"
    "\tmov r8,#0\n"
    "\tadcs r8,r8,r8\n"
    "\tsubs r0,#0xffffffff\n"
    "\tsbcs r1,r1,#0xffffffff\n"
    "\tsbcs r2,r2,#0xffffffff\n"
    "\tsbcs r3,r3,#0\n"
    "\tsbcs r4,r4,#0\n"
    "\tsbcs r5,r5,#0\n"
    "\tsbcs r6,r6,#1\n"
    "\tsbcs r7,r7,#0xffffffff\n"
    "\tsbcs r8,r8,#0\n"
    "\tadds r0,r8\n"
    "\tadcs r1,r1,r8\n"
    "\tadcs r2,r2,r8\n"
    "\tadcs r3,r3,#0\n"
    "\tadcs r4,r4,#0\n"
    "\tadcs r5,r5,#0\n"
    "\tadcs r6,r6,r8, lsr #31\n"
    "\tadcs r7,r7,r8\n"
    "\tbx lr\n"
    "\t.size P256_times2, .-P256_times2\n"
    "\t.align 2\n"
    "R2_mod_p:\n"
    "\t.word 3\n"
    "\t.word 0\n"
    "\t.word 0xffffffff\n"
    "\t.word 0xfffffffb\n"
    "\t.word 0xfffffffe\n"
    "\t.word 0xffffffff\n"
    "\t.word 0xfffffffd\n"
    "\t.word 4\n"
    "\t.type P256_to_montgomery, %function\n"
    "P256_to_montgomery:\n"
    "\t.global P256_to_montgomery\n"
    "\tpush {r0,r4-r11,lr}\n"
    "\tadr r2,R2_mod_p\n"
    "\tbl P256_mulmod\n"
    "\tpop {r8}\n"
    "\tstm r8,{r0-r7}\n"
    "\tpop {r4-r11,pc}\n"
    "\t.size P256_to_montgomery, .-P256_to_montgomery\n"
    "\t.type P256_from_montgomery, %function\n"
    "P256_from_montgomery:\n"
    "\t.global P256_from_montgomery\n"
    "\tpush {r0,r4-r11,lr}\n"
    "\tmovs r2,#0\n"
    "\tmovs r3,#0\n"
    "\tpush {r2-r3}\n"
    "\tpush {r2-r3}\n"
    "\tpush {r2-r3}\n"
    "\tmovs r2,#1\n"
    "\tpush {r2-r3}\n"
    "\tmov r2,sp\n"
    "\tbl P256_mulmod\n"
    "\tadd sp,#32\n"
    "\tpop {r8}\n"
    "\tstm r8,{r0-r7}\n"
    "\tpop {r4-r11,pc}\n"
    "\t.size P256_from_montgomery, .-P256_from_montgomery\n"
    "\t.type P256_check_range_p, %function\n"
    "P256_check_range_p:\n"
    "\t.global P256_check_range_p\n"
    "\tpush {r4-r8,lr}\n"
    "\tldm r0,{r1-r8}\n"
    "\tmovs r0,#0xffffffff\n"
    "\tsubs r1,r0\n"
    "\tsbcs r2,r2,r0\n"
    "\tsbcs r3,r3,r0\n"
    "\tsbcs r4,r4,#0\n"
    "\tsbcs r5,r5,#0\n"
    "\tsbcs r6,r6,#0\n"
    "\tsbcs r7,r7,#1\n"
    "\tsbcs r8,r8,r0\n"
    "\tsbcs r0,r0,r0\n"
    "\tlsrs r0,#31\n"
    "\tpop {r4-r8,pc}\n"
    "\t.size P256_check_range_p, .-P256_check_range_p\n"
    "\t.align 2\n"
    "P256_order_mu:\n"
    "\t.word 0xeedf9bfe\n"
    "\t.word 0x012ffd85\n"
    "\t.word 0xdf1a6c21\n"
    "\t.word 0x43190552\n"
    "\t.word 0xffffffff\n"
    "\t.word 0xfffffffe\n"
    "\t.word 0xffffffff\n"
    "\t.word 0x0\n"
    "\t.word 0x1\n"
    "\t.type P256_reduce_mod_n_once, %function\n"
    "P256_reduce_mod_n_once:\n"
    "\tpush {lr}\n"
    "\tadr r10,P256_order\n"
    "\tldm r10,{r10,r11,r12,lr}\n"
    "\tsubs r0,r10\n"
    "\tsbcs r1,r1,r11\n"
    "\tsbcs r2,r2,r12\n"
    "\tsbcs r3,r3,lr\n"
    "\tsbcs r4,r4,#0xffffffff\n"
    "\tsbcs r5,r5,#0xffffffff\n"
    "\tsbcs r6,r6,#0\n"
    "\tsbcs r7,r7,#0xffffffff\n"
    "\tsbcs r8,r8,#0\n"
    "\tsbc r9,r9,r9\n"
    "\tand r10,r9\n"
    "\tand r11,r9\n"
    "\tand r12,r9\n"
    "\tand lr,r9\n"
    "\tadds r0,r10\n"
    "\tadcs r1,r1,r11\n"
    "\tadcs r2,r2,r12\n"
    "\tadcs r3,r3,lr\n"
    "\tadcs r4,r4,r9\n"
    "\tadcs r5,r5,r9\n"
    "\tadcs r6,r6,#0\n"
    "\tadcs r7,r7,r9\n"
    "\tadcs r8,r8,#0\n"
    "\tpop {pc}\n"
    "\t.size P256_reduce_mod_n_once, .-P256_reduce_mod_n_once\n"
    "\t.type P256_reduce_mod_n_64bytes, %function\n"
    "P256_reduce_mod_n_64bytes:\n"
    "\tpush {r0,r4-r11,lr}\n"
    "\tsub sp,sp,#108\n"
    "\tmov r10,r1\n"
    "\tadd r0,sp,#36\n"
    "\tadds r1,r1,#28\n"
    "\tadr r2,P256_order_mu\n"
    "\tbl mul288x288\n"
    "\tmov r0,sp\n"
    "\tadd r1,sp,#72\n"
    "\tadr r2,P256_order\n"
    "\tbl mul288x288\n"
    "\tldm r10,{r0-r8}\n"
    "\tpop {r9-r12}\n"
    "\tsubs r0,r0,r9\n"
    "\tsbcs r1,r1,r10\n"
    "\tsbcs r2,r2,r11\n"
    "\tsbcs r3,r3,r12\n"
    "\tpop {r9-r12,lr}\n"
    "\tsbcs r4,r4,r9\n"
    "\tsbcs r5,r5,r10\n"
    "\tsbcs r6,r6,r11\n"
    "\tsbcs r7,r7,r12\n"
    "\tsbcs r8,r8,lr\n"
    "\tbl P256_reduce_mod_n_once\n"
    "\tbl P256_reduce_mod_n_once\n"
    "\tadd sp,sp,#72\n"
    "\tpop {r9}\n"
    "\tstm r9,{r0-r7}\n"
    "\tpop {r4-r11,pc}\n"
    "\t.size P256_reduce_mod_n_64bytes, .-P256_reduce_mod_n_64bytes\n"
    "\t.type P256_reduce_mod_n_32bytes, %function\n"
    "P256_reduce_mod_n_32bytes:\n"
    "\t.global P256_reduce_mod_n_32bytes\n"
    "\tpush {r0,r4-r11,lr}\n"
    "\tldm r1,{r0-r7}\n"
    "\tmov r8,#0\n"
    "\tbl P256_reduce_mod_n_once\n"
    "\tpop {r8}\n"
    "\tstm r8,{r0-r7}\n"
    "\tpop {r4-r11,pc}\n"
    "\t.size P256_reduce_mod_n_32bytes, .-P256_reduce_mod_n_32bytes\n"
    "\t.type P256_add_mod_n, %function\n"
    "P256_add_mod_n:\n"
    "\t.global P256_add_mod_n\n"
    "\tpush {r0,r4-r11,lr}\n"
    "\tmov r12,r1\n"
    "\tldm r2,{r4-r11}\n"
    "\tldm r12!,{r0-r3}\n"
    "\tadds r0,r4\n"
    "\tadcs r1,r1,r5\n"
    "\tadcs r2,r2,r6\n"
    "\tadcs r3,r3,r7\n"
    "\tldm r12,{r4-r7}\n"
    "\tadcs r4,r4,r8\n"
    "\tadcs r5,r5,r9\n"
    "\tadcs r6,r6,r10\n"
    "\tadcs r7,r7,r11\n"
    "\tmovs r8,#0\n"
    "\tadcs r8,r8,r8\n"
    "\tbl P256_reduce_mod_n_once\n"
    "\tbl P256_reduce_mod_n_once\n"
    "\tpop {r8}\n"
    "\tstm r8,{r0-r7}\n"
    "\tpop {r4-r11,pc}\n"
    "\t.size P256_add_mod_n, .-P256_add_mod_n\n"
    "\t.type P256_mul_mod_n, %function\n"
    "P256_mul_mod_n:\n"
    "\t.global P256_mul_mod_n\n"
    "\tmovs r3,#0\n"
    "\tpush {r3-r10,lr}\n"
    "\tmov r4,r0\n"
    "\tldm r1,{r1,r3,r5-r10}\n"
    "\tpush {r1,r3,r5-r10}\n"
    "\tmovs r1,#0\n"
    "\tpush {r1}\n"
    "\tldm r2,{r1,r3,r5-r10}\n"
    "\tpush {r1,r3,r5-r10}\n"
    "\tsub sp,#72\n"
    "\tmov r0,sp\n"
    "\tadd r1,sp,#72\n"
    "\tadd r2,sp,#108\n"
    "\tbl mul288x288\n"
    "\tmov r0,r4\n"
    "\tmov r1,sp\n"
    "\tbl P256_reduce_mod_n_64bytes\n"
    "\tadd sp,#144\n"
    "\tpop {r4-r10,pc}\n"
    "\t.size P256_mul_mod_n, .-P256_mul_mod_n\n"
    "\t.type P256_divsteps2_31, %function\n"
    "P256_divsteps2_31:\n"
    "\t.global P256_divsteps2_31\n"
    "\tpush {r3,r4-r8,lr}\n"
    "\tmovs r4,#1\n"
    "\tmovs r5,#0\n"
    "\tmovs r6,#0\n"
    "\tmovs r7,#1\n"
    "\tmov lr,#31\n"
    "0:\n"
    "\tsubs r3,r0,#1\n"
    "\tlsl r12,r2,#31\n"
    "\tbic r3,r12,r3\n"
    "\tasrs r3,r3,#31\n"
    "\tlsr r8,r3,#31\n"
    "\teors r0,r0,r3\n"
    "\tsubs r0,r0,r3\n"
    "\tmul r12,r1,r3\n"
    "\tbics r1,r1,r3\n"
    "\tumlal r1,r12,r2,r8\n"
    "\tumaal r2,r12,r2,r3\n"
    "\tmul r12,r4,r3\n"
    "\tbics r4,r4,r3\n"
    "\tumlal r4,r12,r6,r8\n"
    "\tumaal r6,r12,r6,r3\n"
    "\tmul r12,r5,r3\n"
    "\tbics r5,r5,r3\n"
    "\tumlal r5,r12,r7,r8\n"
    "\tumaal r7,r12,r7,r3\n"
    "\tands r12,r2,#1\n"
    "\tadds r0,r0,#1\n"
    "\tmul r3,r12,r1\n"
    "\tadds r2,r2,r3\n"
    "\tlsrs r2,r2,#1\n"
    "\tumlal r6,r8,r12,r4\n"
    "\tumlal r7,r8,r12,r5\n"
    "\tadds r4,r4,r4\n"
    "\tadds r5,r5,r5\n"
    "\tsubs lr,lr,#1\n"
    "\tbne 0b\n"
    "\tpop {r3}\n"
    "\tstm r3!,{r4-r7}\n"
    "\tpop {r4-r8,pc}\n"
    "\t.size P256_divsteps2_31, .-P256_divsteps2_31\n"
    "\t.type P256_matrix_mul_fg_9, %function\n"
    "P256_matrix_mul_fg_9:\n"
    "\t.global P256_matrix_mul_fg_9\n"
    "\tpush {r4-r11,lr}\n"
    "\tand r4,r0,r0,lsl #1\n"
    "\tasrs r4,r4,#31\n"
    "\teors r0,r0,r4\n"
    "\tsubs r0,r0,r4\n"
    "\tand r5,r1,r1,lsl #1\n"
    "\tasrs r5,r5,#31\n"
    "\teors r1,r1,r5\n"
    "\tsubs r1,r1,r5\n"
    "\tldm r2!,{r6}\n"
    "\tldr r7,[r2,#36]\n"
    "\teors r4,r4,r6\n"
    "\teors r5,r5,r7\n"
    "\teors r4,r4,r5\n"
    "\tstm r3!,{r5}\n"
    "\tpush {r1,r2,r3}\n"
    "\tldm r2!,{r1,r3,r5-r11}\n"
    "\teors r1,r1,r4\n"
    "\teors r3,r3,r4\n"
    "\teors r5,r5,r4\n"
    "\teors r6,r6,r4\n"
    "\teors r7,r7,r4\n"
    "\teor r8,r8,r4\n"
    "\teor r9,r9,r4\n"
    "\teor r10,r10,r4\n"
    "\tsubs r1,r1,r4\n"
    "\tsbcs r3,r3,r4\n"
    "\tsbcs r5,r5,r4\n"
    "\tsbcs r6,r6,r4\n"
    "\tsbcs r7,r7,r4\n"
    "\tsbcs r8,r8,r4\n"
    "\tsbcs r9,r9,r4\n"
    "\tsbcs r10,r10,r4\n"
    "\teor r4,r4,r11\n"
    "\tumull r1,lr,r0,r1\n"
    "\tmovs r2,#0\n"
    "\tumull r11,r12,r2,r2\n"
    "\tumaal r2,lr,r0,r3\n"
    "\tumaal r11,lr,r0,r5\n"
    "\tumull r3,r5,r12,r12\n"
    "\tumaal r3,lr,r0,r6\n"
    "\tumaal r5,lr,r0,r7\n"
    "\tumull r6,r7,r12,r12\n"
    "\tumaal r6,lr,r0,r8\n"
    "\tumaal r7,lr,r0,r9\n"
    "\tumaal r12,lr,r0,r10\n"
    "\tmla lr,r0,r4,lr\n"
    "\tpop {r0,r4}\n"
    "\tadds r4,r4,#40\n"
    "\tldm r4!,{r8,r9}\n"
    "\tmov r10,#0\n"
    "\tumaal r1,r10,r0,r8\n"
    "\tumaal r2,r10,r0,r9\n"
    "\tadds r1,r1,r1\n"
    "\tadcs r2,r2,r2\n"
    "\tldm r4!,{r1,r8,r9}\n"
    "\tumaal r10,r11,r0,r1\n"
    "\tumaal r11,r3,r0,r8\n"
    "\tumaal r3,r5,r0,r9\n"
    "\tadcs r10,r10,r10\n"
    "\tadcs r11,r11,r11\n"
    "\tadcs r3,r3,r3\n"
    "\tldm r4,{r1,r4,r8,r9}\n"
    "\tumaal r5,r6,r0,r1\n"
    "\tumaal r6,r7,r0,r4\n"
    "\tumaal r7,r12,r0,r8\n"
    "\tumaal r12,lr,r0,r9\n"
    "\tadcs r5,r5,r5\n"
    "\tadcs r6,r6,r6\n"
    "\tadcs r7,r7,r7\n"
    "\tadcs r12,r12,r12\n"
    "\tsbcs lr,lr,lr\n"
    "\tmvn lr,lr\n"
    "\tpop {r1}\n"
    "\tstm r1!,{r2,r10,r11}\n"
    "\tstm r1!,{r3,r5,r6,r7,r12,lr}\n"
    "\tpop {r4-r11,pc}\n"
    "\t.size P256_matrix_mul_fg_9, .-P256_matrix_mul_fg_9\n"
    "\t.align 2\n"
    "\t.type P256_matrix_mul_mod_n, %function\n"
    "P256_matrix_mul_mod_n:\n"
    "\t.global P256_matrix_mul_mod_n\n"
    "\tpush {r4-r11,lr}\n"
    "\tand r4,r0,r0,lsl #1\n"
    "\tasrs r4,r4,#31\n"
    "\teors r0,r0,r4\n"
    "\tsubs r0,r0,r4\n"
    "\tand r5,r1,r1,lsl #1\n"
    "\tasrs r5,r5,#31\n"
    "\teors r1,r1,r5\n"
    "\tsubs r1,r1,r5\n"
    "\tldm r2!,{r6}\n"
    "\tldr r7,[r2,#32]\n"
    "\teors r4,r4,r6\n"
    "\teors r5,r5,r7\n"
    "\teors r4,r4,r5\n"
    "\tstm r3!,{r5}\n"
    "\tpush {r1,r2,r3}\n"
    "\tldm r2,{r1-r3,r5-r9}\n"
    "\teors r1,r1,r4\n"
    "\teors r2,r2,r4\n"
    "\teors r3,r3,r4\n"
    "\teors r5,r5,r4\n"
    "\teors r6,r6,r4\n"
    "\teors r7,r7,r4\n"
    "\teor r8,r8,r4\n"
    "\teor r9,r9,r4\n"
    "\tsubs r1,r1,r4\n"
    "\tsbcs r2,r2,r4\n"
    "\tsbcs r3,r3,r4\n"
    "\tsbcs r5,r5,r4\n"
    "\tsbcs r6,r6,r4\n"
    "\tsbcs r7,r7,r4\n"
    "\tsbcs r8,r8,r4\n"
    "\tsbcs r9,r9,r4\n"
    "\tsbcs r4,r4,r4\n"
    "\tlsrs lr,r4,#31\n"
    "\tmov r12,#0\n"
    "\tldrd r10,r11,P256_order_local\n"
    "\tumaal r1,r12,lr,r10\n"
    "\tumaal r2,r12,lr,r11\n"
    "\tldrd r10,r11,P256_order_local+8\n"
    "\tumaal r3,r12,lr,r10\n"
    "\tumaal r5,r12,lr,r11\n"
    "\tumaal r6,r12,lr,r4\n"
    "\tumaal r7,r12,lr,r4\n"
    "\tmov r10,#0\n"
    "\tumaal r8,r12,lr,r10\n"
    "\tumaal r9,r12,lr,r4\n"
    "\tumull r11,lr,r10,r10\n"
    "\tumull r10,lr,r0,r1\n"
    "\tumull r1,r12,r11,r11\n"
    "\tumaal r11,lr,r0,r2\n"
    "\tumaal r1,lr,r0,r3\n"
    "\tumull r2,r3,r12,r12\n"
    "\tumaal r2,lr,r0,r5\n"
    "\tumaal r3,lr,r0,r6\n"
    "\tumull r4,r5,r12,r12\n"
    "\tumaal r4,lr,r0,r7\n"
    "\tumaal r5,lr,r0,r8\n"
    "\tumaal r12,lr,r0,r9\n"
    "\tpop {r0,r6}\n"
    "\tadds r6,r6,#36\n"
    "\tldm r6!,{r8,r9}\n"
    "\tmovs r7,#0\n"
    "\tumaal r10,r7,r0,r8\n"
    "\tumaal r11,r7,r0,r9\n"
    "\tldm r6!,{r8,r9}\n"
    "\tumaal r1,r7,r0,r8\n"
    "\tumaal r2,r7,r0,r9\n"
    "\tldm r6!,{r8,r9}\n"
    "\tumaal r3,r7,r0,r8\n"
    "\tumaal r4,r7,r0,r9\n"
    "\tldm r6!,{r8,r9}\n"
    "\tumaal r5,r7,r0,r8\n"
    "\tumaal r12,r7,r0,r9\n"
    "\tadd lr,lr,r7\n"
    "\tldr r0,=0xee00bc4f\n"
    "\tmul r0,r10,r0\n"
    "\tmovs r6,#0\n"
    "\tldrd r8,r9,P256_order_local\n"
    "\tumaal r10,r6,r0,r8\n"
    "\tumaal r11,r6,r0,r9\n"
    "\tsubs r11,r11,r8\n"
    "\tldrd r8,r10,P256_order_local+8\n"
    "\tumaal r1,r6,r0,r8\n"
    "\tsbcs r1,r1,r9\n"
    "\tumaal r2,r6,r0,r10\n"
    "\tmov r9,#-1\n"
    "\tumaal r3,r6,r0,r9\n"
    "\tumaal r4,r6,r0,r9\n"
    "\tmovs r7,#0\n"
    "\tumaal r5,r6,r0,r7\n"
    "\tumaal r12,r6,r0,r9\n"
    "\tumaal lr,r6,r7,r7\n"
    "\tsbcs r2,r2,r8\n"
    "\tsbcs r3,r3,r10\n"
    "\tsbcs r4,r4,r9\n"
    "\tsbcs r5,r5,r9\n"
    "\tsbcs r12,r12,r7\n"
    "\tsbcs lr,lr,r9\n"
    "\tsbcs r6,r6,r7\n"
    "\tldrd r0,r9,P256_order_local\n"
    "\tlsrs r6,r6,#31\n"
    "\tumaal r7,r11,r6,r0\n"
    "\tumaal r1,r11,r6,r9\n"
    "\tumaal r2,r11,r6,r8\n"
    "\tumaal r3,r11,r6,r10\n"
    "\trsbs r0,r6,#0\n"
    "\tumaal r4,r11,r6,r0\n"
    "\tumaal r5,r11,r6,r0\n"
    "\tmov r8,#0\n"
    "\tumaal r11,r12,r6,r8\n"
    "\tumaal r12,lr,r6,r0\n"
    "\tpop {r6}\n"
    "\tstm r6!,{r7}\n"
    "\tstm r6!,{r1,r2,r3,r4,r5,r11,r12}\n"
    "\tpop {r4-r11,pc}\n"
    "\t.ltorg\n"
    "\t.size P256_matrix_mul_mod_n, .-P256_matrix_mul_mod_n\n"
    "\t.align 2\n"
    "P256_order_local:\n"
    "\t.type P256_order, %object\n"
    "P256_order:\n"
    "\t.global P256_order\n"
    "\t.word 0xFC632551\n"
    "\t.word 0xF3B9CAC2\n"
    "\t.word 0xA7179E84\n"
    "\t.word 0xBCE6FAAD\n"
    "\t.word 0xFFFFFFFF\n"
    "\t.word 0xFFFFFFFF\n"
    "\t.word 0\n"
    "\t.word 0xFFFFFFFF\n"
    "\t.word 0\n"
    "\t.size P256_order, .-P256_order\n"
    "\t.type P256_check_range_n, %function\n"
    "P256_check_range_n:\n"
    "\t.global P256_check_range_n\n"
    "\tpush {r4-r11,lr}\n"
    "\tldm r0,{r1-r8}\n"
    "\torrs r0,r1,r2\n"
    "\torrs r0,r3\n"
    "\torrs r0,r4\n"
    "\torrs r0,r5\n"
    "\torrs r0,r6\n"
    "\torrs r0,r7\n"
    "\torrs r0,r8\n"
    "\tbeq 0f\n"
    "\tadr r0,P256_order\n"
    "\tldm r0!,{r9-r12}\n"
    "\tsubs r1,r9\n"
    "\tsbcs r2,r2,r10\n"
    "\tsbcs r3,r3,r11\n"
    "\tsbcs r4,r4,r12\n"
    "\tldm r0,{r0-r3}\n"
    "\tsbcs r5,r5,r0\n"
    "\tsbcs r6,r6,r1\n"
    "\tsbcs r7,r7,r2\n"
    "\tsbcs r8,r8,r3\n"
    "\tsbcs r0,r0,r0\n"
    "\tlsrs r0,#31\n"
    "0:\n"
    "\tpop {r4-r11,pc}\n"
    "\t.size P256_check_range_n, .-P256_check_range_n\n"
    "\t.align 2\n"
    "b_mont:\n"
    "\t.word 0x29c4bddf\n"
    "\t.word 0xd89cdf62\n"
    "\t.word 0x78843090\n"
    "\t.word 0xacf005cd\n"
    "\t.word 0xf7212ed6\n"
    "\t.word 0xe5a220ab\n"
    "\t.word 0x04874834\n"
    "\t.word 0xdc30061d\n"
    "three_mont:\n"
    "\t.word 0x3\n"
    "\t.word 0x0\n"
    "\t.word 0x0\n"
    "\t.word 0xfffffffd\n"
    "\t.word 0xffffffff\n"
    "\t.word 0xffffffff\n"
    "\t.word 0xfffffffc\n"
    "\t.word 0x2\n"
    "\t.type P256_point_is_on_curve, %function\n"
    "P256_point_is_on_curve:\n"
    "\t.global P256_point_is_on_curve\n"
    "\tpush {r0,r4-r11,lr}\n"
    "\tldm r1,{r0-r7}\n"
    "\tbl P256_sqrmod\n"
    "\tpush {r0-r7}\n"
    "\tldr r0,[sp,#32]\n"
    "\tldm r0,{r0-r7}\n"
    "\tbl P256_sqrmod\n"
    "\tpush {r0-r7}\n"
    "\tmov r1,sp\n"
    "\tadr r2,three_mont\n"
    "\tbl P256_submod\n"
    "\tstm sp,{r0-r7}\n"
    "\tldr r1,[sp,#64]\n"
    "\tmov r2,sp\n"
    "\tbl P256_mulmod\n"
    "\tstm sp,{r0-r7}\n"
    "\tadd r1,sp,#32\n"
    "\tmov r2,sp\n"
    "\tbl P256_submod\n"
    "\tadr r8,b_mont\n"
    "\tldm r8!,{r9-r12}\n"
    "\teors r0,r9\n"
    "\tittt eq\n"
    "\teorseq r1,r10\n"
    "\teorseq r2,r11\n"
    "\teorseq r3,r12\n"
    "\tldm r8,{r9-r12}\n"
    "\titttt eq\n"
    "\teorseq r4,r9\n"
    "\teorseq r5,r10\n"
    "\teorseq r6,r11\n"
    "\teorseq r7,r12\n"
    "\tmov r0,#0\n"
    "\tit eq\n"
    "\tmoveq r0,#1\n"
    "\tadd sp,#68\n"
    "\tpop {r4-r11,pc}\n"
    "\t.size P256_point_is_on_curve, .-P256_point_is_on_curve\n"
    "\t.align 2\n"
    "P256_p:\n"
    "\t.word 0xffffffff\n"
    "\t.word 0xffffffff\n"
    "\t.word 0xffffffff\n"
    "\t.word 0\n"
    "\t.word 0\n"
    "\t.word 0\n"
    "\t.word 1\n"
    "\t.word 0xffffffff\n"
    "\t.type P256_jacobian_to_affine, %function\n"
    "P256_jacobian_to_affine:\n"
    "\t.global P256_jacobian_to_affine\n"
    "\tpush {r0,r1,r2,r4-r11,lr}\n"
    "\tadds r2,#64\n"
    "\tldm r2,{r0-r7}\n"
    "\tmov r8,#0\n"
    "\tbl P256_modinv_sqrt\n"
    "\tpush {r0-r7}\n"
    "\tbl P256_sqrmod\n"
    "\tpush {r0-r7}\n"
    "\tadd r1,sp,#32\n"
    "\tmov r2,sp\n"
    "\tbl P256_mulmod\n"
    "\tadd r8,sp,#32\n"
    "\tstm r8,{r0-r7}\n"
    "\tmov r1,sp\n"
    "\tldr r2,[sp,#72]\n"
    "\tbl P256_mulmod\n"
    "\tldr r8,[sp,#64]\n"
    "\tstm r8,{r0-r7}\n"
    "\tldr r2,[sp,#72]\n"
    "\tadd r1,sp,#32\n"
    "\tadds r2,r2,#32\n"
    "\tbl P256_mulmod\n"
    "\tldr r8,[sp,#68]\n"
    "\tstm r8,{r0-r7}\n"
    "\tadd sp,#76\n"
    "\tpop {r4-r11,pc}\n"
    "\t.size P256_jacobian_to_affine, .-P256_jacobian_to_affine\n"
    "\t.type P256_double_j, %function\n"
    "P256_double_j:\n"
    "\t.global P256_double_j\n"
    "\tpush {r0,r1,r4-r11,lr}\n"
    "\tadds r1,#64\n"
    "\tldm r1,{r0-r7}\n"
    "\tbl P256_sqrmod\n"
    "\tpush {r0-r7}\n"
    "\tldr r1,[sp,#36]\n"
    "\tadds r1,#32\n"
    "\tadd r2,r1,#32\n"
    "\tbl P256_mulmod\n"
    "\tldr r8,[sp,#32]\n"
    "\tadd r8,#64\n"
    "\tstm r8,{r0-r7}\n"
    "\tldr r1,[sp,#36]\n"
    "\tmov r2,sp\n"
    "\tbl P256_addmod\n"
    "\tpush {r0-r7}\n"
    "\tldr r1,[sp,#68]\n"
    "\tadd r2,sp,#32\n"
    "\tbl P256_submod\n"
    "\tadd r8,sp,#32\n"
    "\tstm r8,{r0-r7}\n"
    "\tadd r1,sp,#32\n"
    "\tmov r2,sp\n"
    "\tbl P256_mulmod\n"
    "\tadd r8,sp,#32\n"
    "\tstm r8,{r0-r7}\n"
    "\tlsl r8,r0,#31\n"
    "\tadds r0,r0,r8, asr #31\n"
    "\tadcs r1,r1,r8, asr #31\n"
    "\tadcs r2,r2,r8, asr #31\n"
    "\tadcs r3,r3,#0\n"
    "\tadcs r4,r4,#0\n"
    "\tadcs r5,r5,#0\n"
    "\tadcs r6,r6,r8, lsr #31\n"
    "\tadcs r7,r7,r8, asr #31\n"
    "\trrxs r7,r7\n"
    "\trrxs r6,r6\n"
    "\trrxs r5,r5\n"
    "\trrxs r4,r4\n"
    "\trrxs r3,r3\n"
    "\trrxs r2,r2\n"
    "\trrxs r1,r1\n"
    "\trrx r0,r0\n"
    "\tstm sp,{r0-r7}\n"
    "\tadd r1,sp,#32\n"
    "\tmov r2,sp\n"
    "\tbl P256_addmod\n"
    "\tadd r8,sp,#32\n"
    "\tstm r8,{r0-r7}\n"
    "\tbl P256_sqrmod\n"
    "\tstm sp,{r0-r7}\n"
    "\tldr r0,[sp,#68]\n"
    "\tadds r0,#32\n"
    "\tldm r0,{r0-r7}\n"
    "\tbl P256_sqrmod\n"
    "\tldr r8,[sp,#64]\n"
    "\tadd r8,#32\n"
    "\tstm r8,{r0-r7}\n"
    "\tbl P256_sqrmod\n"
    "\tpush {r0-r7}\n"
    "\tldrd r0,r1,[sp,#96]\n"
    "\tadd r2,r0,#32\n"
    "\tbl P256_mulmod\n"
    "\tldr r8,[sp,#96]\n"
    "\tadd r8,#32\n"
    "\tstm r8,{r0-r7}\n"
    "\tbl P256_times2\n"
    "\tldr r8,[sp,#96]\n"
    "\tstm r8,{r0-r7}\n"
    "\tadd r1,sp,#32\n"
    "\tmov r2,r8\n"
    "\tbl P256_submod\n"
    "\tldr r8,[sp,#96]\n"
    "\tstm r8,{r0-r7}\n"
    "\tmov r2,r8\n"
    "\tadd r1,r2,#32\n"
    "\tbl P256_submod\n"
    "\tadd r8,sp,#32\n"
    "\tstm r8,{r0-r7}\n"
    "\tadd r1,sp,#64\n"
    "\tadd r2,sp,#32\n"
    "\tbl P256_mulmod\n"
    "\tadd r8,sp,#64\n"
    "\tstm r8,{r0-r7}\n"
    "\tadd r1,sp,#64\n"
    "\tmov r2,sp\n"
    "\tbl P256_submod\n"
    "\tldr r8,[sp,#96]\n"
    "\tadd r8,#32\n"
    "\tstm r8,{r0-r7}\n"
    "\tadd sp,#104\n"
    "\tpop {r4-r11,pc}\n"
    "\t.size P256_double_j, .-P256_double_j\n"
    "\t.type add_sub_helper, %function\n"
    "add_sub_helper:\n"
    "\tpush {lr}\n"
    "\tldm r1!,{r5-r12}\n"
    "\tstm r0!,{r5-r12}\n"
    "\tldm r1!,{r5-r12}\n"
    "\tcbz r2,0f\n"
    "\tmov lr,#0\n"
    "\trsbs r4,r2,#0\n"
    "\tsubs r5,r4,r5\n"
    "\tsbcs r6,r4,r6\n"
    "\tsbcs r7,r4,r7\n"
    "\tsbcs r8,lr,r8\n"
    "\tsbcs r9,lr,r9\n"
    "\tsbcs r10,lr,r10\n"
    "\tsbcs r11,r2,r11\n"
    "\tsbcs r12,r4,r12\n"
    "0:\n"
    "\tstm r0!,{r5-r12}\n"
    "\tcbnz r3,1f\n"
    "\tldm r1,{r5-r12}\n"
    "\tstm r0,{r5-r12}\n"
    "\tb 2f\n"
    "1:\n"
    "\tmovs r4,#0\n"
    "\tumull r5,r10,r4,r4\n"
    "\tmvns r6,r4\n"
    "\tmvns r7,r4\n"
    "\tmov r8,#0xffffffff\n"
    "\tmov r9,#0xfffffffe\n"
    "\tstm r0,{r3-r10}\n"
    "2:\n"
    "\tpop {pc}\n"
    "\t.size add_sub_helper, .-add_sub_helper\n"
    "\t.type P256_add_sub_j, %function\n"
    "P256_add_sub_j:\n"
    "\t.global P256_add_sub_j\n"
    "\tpush {r0-r11,lr}\n"
    "\tadd r4,r0,#64\n"
    "\tldm r4,{r4-r11}\n"
    "\torrs r4,r5\n"
    "\torrs r4,r6\n"
    "\torrs r4,r7\n"
    "\torrs r4,r8\n"
    "\torrs r4,r9\n"
    "\torrs r4,r10\n"
    "\torrs r4,r11\n"
    "\tbne 2f\n"
    "\tbl add_sub_helper\n"
    "\tadd sp,#16\n"
    "\tpop {r4-r11,pc}\n"
    "2:\n"
    "\tcbnz r3,100f\n"
    "\tadds r1,#64\n"
    "\tldm r1,{r0-r7}\n"
    "\tbl P256_sqrmod\n"
    "\tpush {r0-r7}\n"
    "\tldr r1,[sp,#32]\n"
    "\tmov r2,sp\n"
    "\tbl P256_mulmod\n"
    "\tldr r8,[sp,#32]\n"
    "\tstm r8,{r0-r7}\n"
    "\tldr r1,[sp,#36]\n"
    "\tadds r1,#64\n"
    "\tmov r2,sp\n"
    "\tbl P256_mulmod\n"
    "\tstm sp,{r0-r7}\n"
    "\tldr r1,[sp,#32]\n"
    "\tadds r1,#32\n"
    "\tmov r2,sp\n"
    "\tbl P256_mulmod\n"
    "\tldr r8,[sp,#32]\n"
    "\tadd r8,#32\n"
    "\tstm r8,{r0-r7}\n"
    "\tb 101f\n"
    "100:\n"
    "\tsub sp,#32\n"
    "101:\n"
    "\tldr r1,[sp,#32]\n"
    "\tadds r1,#64\n"
    "\tldm r1,{r0-r7}\n"
    "\tbl P256_sqrmod\n"
    "\tpush {r0-r7}\n"
    "\tldr r1,[sp,#68]\n"
    "\tmov r2,sp\n"
    "\tbl P256_mulmod\n"
    "\tadd r8,sp,#32\n"
    "\tstm r8,{r0-r7}\n"
    "\tldr r1,[sp,#64]\n"
    "\tadds r1,#64\n"
    "\tmov r2,sp\n"
    "\tbl P256_mulmod\n"
    "\tstm sp,{r0-r7}\n"
    "\tldr r1,[sp,#68]\n"
    "\tadds r1,#32\n"
    "\tmov r2,sp\n"
    "\tbl P256_mulmod\n"
    "\tstm sp,{r0-r7}\n"
    "\tldr r1,[sp,#64]\n"
    "\tadd r2,sp,#32\n"
    "\tbl P256_submod\n"
    "\tldr r8,[sp,#64]\n"
    "\tstm r8,{r0-r7}\n"
    "\tbl P256_sqrmod\n"
    "\tpush {r0-r7}\n"
    "\tldr r2,[sp,#96]\n"
    "\tadd r1,r2,#64\n"
    "\tbl P256_mulmod\n"
    "\tldr r8,[sp,#96]\n"
    "\tadd r8,#64\n"
    "\tstm r8,{r0-r7}\n"
    "\tldr r1,[sp,#108]\n"
    "\tcbnz r1,102f\n"
    "\tldr r1,[sp,#100]\n"
    "\tadds r1,#64\n"
    "\tmov r2,r8\n"
    "\tbl P256_mulmod\n"
    "\tldr r8,[sp,#96]\n"
    "\tadd r8,#64\n"
    "\tstm r8,{r0-r7}\n"
    "102:\n"
    "\tldr r1,[sp,#96]\n"
    "\tmov r2,sp\n"
    "\tbl P256_mulmod\n"
    "\tldr r8,[sp,#96]\n"
    "\tstm r8,{r0-r7}\n"
    "\torrs r1,r0\n"
    "\torrs r1,r2\n"
    "\torrs r1,r3\n"
    "\torrs r1,r4\n"
    "\torrs r1,r5\n"
    "\torrs r1,r6\n"
    "\torrs r0,r1,r7\n"
    "3:\n"
    "\tpush {r0}\n"
    "\tldr r1,[sp,#100]\n"
    "\tadds r1,#32\n"
    "\tadd r2,sp,#36\n"
    "\tldr r3,[sp,#108]\n"
    "\tcbz r3,4f\n"
    "\tbl P256_addmod\n"
    "\tb 5f\n"
    "4:\n"
    "\tbl P256_submod\n"
    "5:\n"
    "\tldr r8,[sp,#100]\n"
    "\tadd r8,#32\n"
    "\tstm r8,{r0-r7}\n"
    "\tpop {r8}\n"
    "\torrs r1,r0\n"
    "\torrs r1,r2\n"
    "\torrs r1,r3\n"
    "\torrs r1,r4\n"
    "\torrs r1,r5\n"
    "\torrs r1,r6\n"
    "\torrs r1,r7\n"
    "\torrs r1,r8\n"
    "\tbne 6f\n"
    "\tadd sp,#96\n"
    "\tldm sp,{r0-r3}\n"
    "\tbl add_sub_helper\n"
    "\tldr r0,[sp,#0]\n"
    "\tmov r1,r0\n"
    "\tadd sp,#16\n"
    "\tbl P256_double_j\n"
    "\tpop {r4-r11,pc}\n"
    "6:\n"
    "\tadd r1,sp,#64\n"
    "\tmov r2,sp\n"
    "\tbl P256_mulmod\n"
    "\tadd r8,sp,#64\n"
    "\tstm r8,{r0-r7}\n"
    "\tldr r0,[sp,#96]\n"
    "\tadds r0,#32\n"
    "\tldm r0,{r0-r7}\n"
    "\tbl P256_sqrmod\n"
    "\tstm sp,{r0-r7}\n"
    "\tadd r1,sp,#32\n"
    "\tldr r2,[sp,#96]\n"
    "\tbl P256_mulmod\n"
    "\tadd r8,sp,#32\n"
    "\tstm r8,{r0-r7}\n"
    "\tmov r1,sp\n"
    "\tldr r2,[sp,#96]\n"
    "\tbl P256_submod\n"
    "\tldr r8,[sp,#96]\n"
    "\tstm r8,{r0-r7}\n"
    "\tadd r0,sp,#64\n"
    "\tldm r0,{r0-r7}\n"
    "\tbl P256_times2\n"
    "\tstm sp,{r0-r7}\n"
    "\tldr r1,[sp,#96]\n"
    "\tmov r2,sp\n"
    "\tbl P256_submod\n"
    "\tldr r8,[sp,#96]\n"
    "\tstm r8,{r0-r7}\n"
    "\tadd r1,sp,#64\n"
    "\tldr r2,[sp,#96]\n"
    "\tbl P256_submod\n"
    "\tstm sp,{r0-r7}\n"
    "\tldr r1,[sp,#96]\n"
    "\tadds r1,#32\n"
    "\tmov r2,sp\n"
    "\tbl P256_mulmod\n"
    "\tstm sp,{r0-r7}\n"
    "\tldr r0,[sp,#104]\n"
    "\tmov r1,sp\n"
    "\tadd r2,sp,#32\n"
    "\tcbz r0,7f\n"
    "\tbl P256_addmod\n"
    "\tb 8f\n"
    "7:\n"
    "\tbl P256_submod\n"
    "8:\n"
    "\tldr r8,[sp,#96]\n"
    "\tadd r8,#32\n"
    "\tstm r8,{r0-r7}\n"
    "\tadd sp,#112\n"
    "\tpop {r4-r11,pc}\n"
    "\t.size P256_add_sub_j, .-P256_add_sub_j\n"
    "\t.type P256_verify_last_step, %function\n"
    "P256_verify_last_step:\n"
    "\t.global P256_verify_last_step\n"
    "\tpush {r0,r1,r4-r11,lr}\n"
    "\tsub sp,#32\n"
    "\tadd r1,#64\n"
    "\tldm r1,{r0-r7}\n"
    "\tbl P256_sqrmod\n"
    "\tpush {r0-r7}\n"
    "\torrs r0,r1\n"
    "\torrs r0,r2\n"
    "\torrs r0,r3\n"
    "\torrs r0,r4\n"
    "\torrs r0,r5\n"
    "\torrs r0,r6\n"
    "\torrs r0,r7\n"
    "\tbeq 0f\n"
    "\tldr r1,[sp,#64]\n"
    "2:\n"
    "\tadd r0,sp,#32\n"
    "\tbl P256_to_montgomery\n"
    "\tadd r1,sp,#32\n"
    "\tmov r2,sp\n"
    "\tbl P256_mulmod\n"
    "\tldr r8,[sp,#68]\n"
    "\tldm r8!,{r9-r12}\n"
    "\teors r0,r9\n"
    "\tittt eq\n"
    "\teorseq r1,r10\n"
    "\teorseq r2,r11\n"
    "\teorseq r3,r12\n"
    "\tldm r8!,{r9-r12}\n"
    "\titttt eq\n"
    "\teorseq r4,r9\n"
    "\teorseq r5,r10\n"
    "\teorseq r6,r11\n"
    "\teorseq r7,r12\n"
    "\tmov r0,#1\n"
    "\tbeq 1f\n"
    "\tadr r0,P256_order\n"
    "\tldm r0,{r8-r11}\n"
    "\tldr r0,[sp,#64]\n"
    "\tcbz r0,0f\n"
    "\tldm r0,{r0-r7}\n"
    "\tadds r0,r8\n"
    "\tadcs r1,r1,r9\n"
    "\tadcs r2,r2,r10\n"
    "\tadcs r3,r3,r11\n"
    "\tadcs r4,r4,#0xffffffff\n"
    "\tadcs r5,r5,#0xffffffff\n"
    "\tadcs r6,r6,#0\n"
    "\tadcs r7,r7,#0xffffffff\n"
    "\tbcs 0f\n"
    "\tsubs r8,r0,#0xffffffff\n"
    "\tsbcs r8,r1,#0xffffffff\n"
    "\tsbcs r8,r2,#0xffffffff\n"
    "\tsbcs r8,r3,#0\n"
    "\tsbcs r8,r4,#0\n"
    "\tsbcs r8,r5,#0\n"
    "\tsbcs r8,r6,#1\n"
    "\tsbcs r8,r7,#0xffffffff\n"
    "\tbcs 0f\n"
    "\tadd r8,sp,#32\n"
    "\tstm r8,{r0-r7}\n"
    "\tmovs r2,#0\n"
    "\tstr r2,[sp,#64]\n"
    "\tmov r1,r8\n"
    "\tb 2b\n"
    "0:\n"
    "\tmovs r0,#0\n"
    "1:\n"
    "\tadd sp,#72\n"
    "\tpop {r4-r11,pc}\n"
    "\t.size P256_verify_last_step, .-P256_verify_last_step\n"
    "\t.type P256_negate_mod_m_if, %function\n"
    "P256_negate_mod_m_if:\n"
    "\tpush {r4-r8,lr}\n"
    "\trsb r8,r2,#1\n"
    "\tmovs r6,#8\n"
    "\tsubs r7,r7\n"
    "0:\n"
    "\tldm r1!,{r4,r12}\n"
    "\tldm r3!,{r5,lr}\n"
    "\tsbcs r5,r5,r4\n"
    "\tumull r4,r7,r8,r4\n"
    "\tumaal r4,r7,r2,r5\n"
    "\tsbcs lr,lr,r12\n"
    "\tumull r12,r7,r8,r12\n"
    "\tumaal r12,r7,r2,lr\n"
    "\tstm r0!,{r4,r12}\n"
    "\tsub r6,#2\n"
    "\tcbz r6,1f\n"
    "\tb 0b\n"
    "1:\n"
    "\tpop {r4-r8,pc}\n"
    "\t.size P256_negate_mod_m_if, .-P256_negate_mod_m_if\n"
    "\t.type P256_negate_mod_n_if, %function\n"
    "P256_negate_mod_n_if:\n"
    "\t.global P256_negate_mod_n_if\n"
    "\tldr r3,=P256_order\n"
    "\tb P256_negate_mod_m_if\n"
    "\t.size P256_negate_mod_n_if, .-P256_negate_mod_n_if\n"
    "\t.type P256_negate_mod_p_if, %function\n"
    "P256_negate_mod_p_if:\n"
    "\t.global P256_negate_mod_p_if\n"
    "\tadr r3,P256_p\n"
    "\tb P256_negate_mod_m_if\n"
    "\t.size P256_negate_mod_p_if, .-P256_negate_mod_p_if\n"
    "\t.align 2\n"
    ".popsection\n");  // No .end here: it would drop the C code that follows

// mg_p256_*() wrappers, see tls_p256.h. The API uses big endian byte strings,
// this library uses little endian 32-bit words, so convert both ways

int mg_p256_make_key(uint8_t public_key[64], uint8_t private_key[32]) {
  uint32_t x[8], y[8], d[8];
  int tries, ok = 0;
  for (tries = 0; tries < 64 && !ok; tries++) {
    if (!mg_random(d, sizeof(d))) break;
    ok = p256_keygen(x, y, d);
  }
  if (ok) {
    p256_convert_endianness(public_key, x, 32);
    p256_convert_endianness(public_key + 32, y, 32);
    p256_convert_endianness(private_key, d, 32);
  }
  mg_bzero((unsigned char *) d, sizeof(d));
  return ok;
}

int mg_p256_compute_public_key(const uint8_t private_key[32],
                               uint8_t public_key[64]) {
  uint32_t x[8], y[8], d[8];
  int ok;
  p256_convert_endianness(d, private_key, 32);
  ok = p256_keygen(x, y, d);  // False if the key is out of range
  if (ok) {
    p256_convert_endianness(public_key, x, 32);
    p256_convert_endianness(public_key + 32, y, 32);
  }
  mg_bzero((unsigned char *) d, sizeof(d));
  return ok;
}

int mg_p256_shared_secret(const uint8_t public_key[64],
                          const uint8_t private_key[32], uint8_t secret[32]) {
  uint32_t x[8], y[8], d[8];
  int ok;
  p256_convert_endianness(x, public_key, 32);
  p256_convert_endianness(y, public_key + 32, 32);
  p256_convert_endianness(d, private_key, 32);
  ok = p256_ecdh_calc_shared_secret(secret, d, x, y);  // Validates the point
  mg_bzero((unsigned char *) d, sizeof(d));
  return ok;
}

int mg_p256_sign_with_k(const uint8_t private_key[32], const uint8_t *hash,
                        size_t hash_size, const uint8_t k[32],
                        uint8_t signature[64]) {
  uint32_t r[8], s[8], d[8], kw[8];
  int ok;
  p256_convert_endianness(d, private_key, 32);
  p256_convert_endianness(kw, k, 32);
  ok = p256_sign(r, s, hash, (uint32_t) hash_size, d, kw);  // False if k is
                                                             // out of range
  if (ok) {
    p256_convert_endianness(signature, r, 32);
    p256_convert_endianness(signature + 32, s, 32);
  }
  mg_bzero((unsigned char *) d, sizeof(d));
  mg_bzero((unsigned char *) kw, sizeof(kw));
  return ok;
}

int mg_p256_verify(const uint8_t public_key[64], const uint8_t *hash,
                   size_t hash_size, const uint8_t signature[64]) {
  uint32_t x[8], y[8], r[8], s[8];
  p256_convert_endianness(x, public_key, 32);
  p256_convert_endianness(y, public_key + 32, 32);
  p256_convert_endianness(r, signature, 32);
  p256_convert_endianness(s, signature + 32, 32);
  return p256_verify(x, y, hash, (uint32_t) hash_size, r, s);  // Checks point
}
#undef get_bit
#endif  // MG_P256 == MG_P256_LENNGREN
