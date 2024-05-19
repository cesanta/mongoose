// Copyright (c) 2013-2024 Cesanta Software Limited

const Pack = async function(files) {
  let out = `// DO NOT EDIT. This file is generated
#include <stddef.h>
#include <string.h>
#include <time.h>

#if defined(__cplusplus)
extern "C" {
#endif

const char *mg_unlist(size_t no);
const char *mg_unpack(const char *, size_t *, time_t *);

#if defined(__cplusplus)
}
#endif
`;
  //const str2bytes = str => new TextEncoder().encode(str);
  let no = 1;
  for (const f of files) {
    let byteArray = new TextEncoder().encode(f.data);
    if (f.zip) {
       const cs = new CompressionStream('gzip');
       const writer = cs.writable.getWriter();
       writer.write(byteArray);
       writer.close();
       const resp = new Response(cs.readable);
       const arr = await resp.arrayBuffer();
       byteArray = new Uint8Array(arr);
    }
    const bytes = Array.from(byteArray);

    // console.log(f.path, bytes);
    out += `
static const unsigned char v${no}[] = {`;
    // concat(0) appends trailing 0, in order to make any file an asciz string
    out += bytes.concat(0).join(',');
    out +=`};`;
    no++;
  }

  out += `

static const struct packed_file {
  const char *name;
  const unsigned char *data;
  size_t size;
  time_t mtime;
} packed_files[] = {`;

  no = 1;
  for (const f of files) {
    out += `
  {"/${f.path}${f.zip ? '.gz' : ''}", v${no}, sizeof(v${no}), 0},`;
    no++;
  }
    out +=`
  {NULL, NULL, 0, 0}
};
`;

  out += `
static int scmp(const char *a, const char *b) {
  while (*a && (*a == *b)) a++, b++;
  return *(const unsigned char *) a - *(const unsigned char *) b;
}

const char *mg_unlist(size_t no) {
  return packed_files[no].name;
}

const char *mg_unpack(const char *name, size_t *size, time_t *mtime) {
  const struct packed_file *p;
  for (p = packed_files; p->name != NULL; p++) {
    if (scmp(p->name, name) != 0) continue;
    if (size != NULL) *size = p->size - 1;
    if (mtime != NULL) *mtime = p->mtime;
    return (const char *) p->data;
  }
  return NULL;
};
`;

  return out;
};

// Script entry point

const fs = require('fs');
const path = require('path');
const [, , ...args] = process.argv;

const files = args.map(f => ({
  path: f,
  data: fs.readFileSync(f, 'utf8'),
  zip: true,
}));

(async () => {
  const text = await Pack(files);
  process.stdout.write(text);
})();

