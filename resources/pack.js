// Copyright (c) 2024 Cesanta Software Limited
//
// Utility that generates packed filesystem C file compatible with the
// Mongoose Network Library, https://github.com/cesanta/mongoose
//
// Usage:
//    node pack.js FILE[:DESTINATION[:gzip]] ...

const fs = require('fs');
const zlib = require('zlib');
const argv = process.argv.slice(2);

// Convert each command-line arguments into [ DATA_ARRAY, STRUCT_INITIALIZER ]
const entries = argv.map(function(filename, i) {
  const parts = filename.split(':');
  const stat = fs.statSync(parts[0]);
  const data = fs.readFileSync(parts[0], null);
  let bytes = Array.from(data);
  let destination = (parts[1] || parts[0]).replace(/^\.+[\/\\]*/, '');
  if (parts[2] == 'gzip') {
    bytes = Array.from(zlib.gzipSync(data));
    destination += '.gz';
  }

  // concat(0) appends trailing 0, in order to make any file an asciz string
  return [
    `static const unsigned char v${i}[] = {${bytes.concat(0).join(',')}};`,
    `  {"/${destination}", v${i}, sizeof(v${i}) - 1, ${parseInt(stat.mtimeMs / 1000)}}`,
  ];
});

process.stdout.write(`// DO NOT EDIT. This file is generated using this command:
// ${process.argv.join(' ')}

#include "mongoose.h"

${entries.map(x => x[0]).join('\n\n')}

const struct mg_mem_file mg_packed_files[] = {
${entries.map(x => x[1]).join(',\n')},
  {NULL, NULL, 0, 0}
};
`);
