#!/usr/bin/env node
// OTA firmware signing tool. No npm dependencies required.
// Usage:
//   node sign.js keygen            # generate private.pem + print public key #define
//   node sign.js sign firmware.bin # append 64-byte ECDSA-P256 signature

'use strict';

const crypto = require('crypto');
const fs = require('fs');
const path = require('path');

function usage() {
  console.error('Usage:');
  console.error('  node sign.js keygen              # generate key pair');
  console.error('  node sign.js sign FIRMWARE.bin   # sign firmware');
  process.exit(1);
}

// DER-encoded ECDSA signature -> raw 64-byte R||S
function derToRaw(der) {
  let i = 2;              // skip SEQUENCE tag + length
  i++;                    // skip INTEGER tag
  const rLen = der[i++];
  const r = der.slice(i, i + rLen); i += rLen;
  i++;                    // skip INTEGER tag
  const sLen = der[i++];
  const s = der.slice(i, i + sLen);
  return Buffer.concat([to32(r), to32(s)]);
}

// Normalize a DER integer to exactly 32 bytes (strip leading 0x00, zero-pad)
function to32(b) {
  while (b.length > 32 && b[0] === 0) b = b.slice(1);
  const out = Buffer.alloc(32, 0);
  b.copy(out, 32 - b.length);
  return out;
}

function keygen() {
  const { privateKey, publicKey } = crypto.generateKeyPairSync('ec', {
    namedCurve: 'P-256',
  });

  fs.writeFileSync('private.pem', privateKey.export({ type: 'pkcs8', format: 'pem' }));
  console.log('Wrote private.pem  (keep secret, never ship in firmware)');

  // SPKI DER for P-256 ends with: 04 [X 32 bytes] [Y 32 bytes]
  // We want the raw 64-byte X||Y (no 0x04 prefix) for mg_uecc_verify
  const spki = publicKey.export({ type: 'spki', format: 'der' });
  const raw = spki.slice(-64);

  const hex = Array.from(raw).map(b => '0x' + b.toString(16).padStart(2, '0'));
  const rows = [];
  for (let i = 0; i < 64; i += 8)
    rows.push('  ' + hex.slice(i, i + 8).join(', '));

  console.log('\nAdd to your mongoose_config.h:');
  console.log(`#define MG_OTA_PUBLIC_KEY { \\\n${rows.join(', \\\n')}, \\\n}`);
}

function sign(firmware, privateKeyFile = 'private.pem') {
  const fw = fs.readFileSync(firmware);
  const privateKey = crypto.createPrivateKey(fs.readFileSync(privateKeyFile));
  const derSig = crypto.sign('SHA256', fw, privateKey);
  const raw = derToRaw(derSig);  // 64 bytes: R||S

  const dot = firmware.lastIndexOf('.');
  const out = dot >= 0
    ? firmware.slice(0, dot) + '.signed' + firmware.slice(dot)
    : firmware + '.signed';

  fs.writeFileSync(out, Buffer.concat([fw, raw]));
  console.log(`Signed: ${out}  (${fw.length} bytes firmware + 64 bytes signature)`);
}

const [,, cmd, arg1, arg2] = process.argv;
if      (cmd === 'keygen')          keygen();
else if (cmd === 'sign' && arg1)    sign(arg1, arg2);
else                                usage();
