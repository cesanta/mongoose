// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
//
// Generate a Mongoose packed filesystem C file from an HTML file after
// inlining referenced CSS, JS, images, and icons.
//
// Usage:
//   node html2c.js [-o OUTPUT.c] HTML_FILE [HTML_FILE ...]

const fs = require('fs');
const path = require('path');
const http = require('http');
const https = require('https');
const zlib = require('zlib');

const args = process.argv.slice(2);
let outputFile = null;
const inputFiles = [];
const useMap = new Map();

for (let i = 0; i < args.length; i++) {
  if (args[i] === '-o' && i + 1 < args.length) {
    outputFile = args[++i];
  } else if (args[i] === '--use' && i + 1 < args.length) {
    const eq = args[++i].indexOf('=');
    if (eq > 0) useMap.set(args[i].slice(0, eq), args[i].slice(eq + 1));
  } else if (!args[i].startsWith('-')) {
    inputFiles.push(args[i]);
  }
}

if (inputFiles.length === 0) {
  console.error('Usage: node html2c.js [-o OUTPUT.c] HTML_FILE [HTML_FILE ...]');
  process.exit(1);
}

function fetchUrl(url) {
  return new Promise((resolve, reject) => {
    const client = url.startsWith('https') ? https : http;
    client
      .get(url, res => {
        const chunks = [];
        res.on('data', chunk => chunks.push(chunk));
        res.on('end', () => resolve(Buffer.concat(chunks)));
        res.on('error', reject);
      })
      .on('error', reject);
  });
}

async function readAsset(name, encoding, baseDir) {
  const override = useMap.get(name);
  if (override != null) name = override;
  if (name.startsWith('http://') || name.startsWith('https://')) {
    const data = await fetchUrl(name);
    return encoding ? data.toString(encoding) : data;
  }
  const fullPath = path.isAbsolute(name) ? name : path.join(baseDir, name);
  return fs.readFileSync(fullPath, encoding || null);
}

function attr(match, name) {
  const re = new RegExp(`${name}=["']([^"']+)["']`, 'i');
  const m = match.match(re);
  return m ? m[1] : null;
}

function replaceAttr(match, name, value) {
  const re = new RegExp(`${name}=(["'])[^"']+\\1`, 'i');
  return match.replace(re, `${name}="${value}"`);
}

async function inlineCss(match, baseDir) {
  const href = attr(match, 'href');
  if (!href) return match;
  const css = await readAsset(href, 'utf8', baseDir);
  return `<style>${css}</style>`;
}

async function inlineJs(match, baseDir) {
  const src = attr(match, 'src');
  if (!src) return match;
  const js = await readAsset(src, 'utf8', baseDir);
  return `<script>${js}</script>`;
}

async function inlineImg(match, baseDir) {
  const src = attr(match, 'src');
  if (!src) return match;
  const ext = path.extname(src).toLowerCase();
  let mime = 'image/png';
  if (ext === '.svg') mime = 'image/svg+xml';
  else if (ext === '.jpg' || ext === '.jpeg') mime = 'image/jpeg';
  else if (ext === '.gif') mime = 'image/gif';
  else if (ext === '.webp') mime = 'image/webp';
  else if (ext === '.ico') mime = 'image/x-icon';

  const data = await readAsset(src, null, baseDir);
  if (ext === '.svg') {
    const svg = Buffer.isBuffer(data) ? data.toString('utf8') : data;
    return replaceAttr(
      match,
      'src',
      `data:${mime};utf8,${encodeURIComponent(svg)}`,
    );
  }
  return replaceAttr(
    match,
    'src',
    `data:${mime};base64,${Buffer.from(data).toString('base64')}`,
  );
}

async function inlineSvg(match, baseDir) {
  const href = attr(match, 'href');
  if (!href || !href.toLowerCase().endsWith('.svg')) return match;
  const data = await readAsset(href, null, baseDir);
  return replaceAttr(
    match,
    'href',
    `data:image/svg+xml;base64,${Buffer.from(data).toString('base64')}`,
  );
}

async function processHtml(content, baseDir) {
  let result = content;
  const cssRegex = /<link[^>]+rel=["']stylesheet["'][^>]*>/g;
  const jsRegex = /<script[^>]+src=["'][^"']+["'][^>]*><\/script>/g;
  const imgRegex = /<img[^>]+src=["'][^"']+["'][^>]*>/g;
  const svgRegex = /<link[^>]+rel=["']icon["'][^>]*>/g;

  for (const match of content.match(cssRegex) || []) {
    if (!match.includes('data:')) result = result.replace(match, await inlineCss(match, baseDir));
  }
  for (const match of content.match(jsRegex) || []) {
    if (!match.includes('data:')) result = result.replace(match, await inlineJs(match, baseDir));
  }
  for (const match of content.match(imgRegex) || []) {
    if (!match.includes('data:')) result = result.replace(match, await inlineImg(match, baseDir));
  }
  for (const match of content.match(svgRegex) || []) {
    if (!match.includes('data:')) result = result.replace(match, await inlineSvg(match, baseDir));
  }

  return result;
}

function cArray(data) {
  return Array.from(Buffer.from(data)).concat(0).join(',');
}

function generateC(files) {
  const vars = files.map(({inputFile, data}, i) => {
    const zipped = zlib.gzipSync(data);
    return {inputFile, zipped};
  });

  const decls = vars.map(({zipped}, i) =>
    `static const unsigned char v${i}[] = {${cArray(zipped)}};`
  ).join('\n');

  const entries = vars.map(({inputFile, zipped}, i) => {
    const destination = `${path.basename(inputFile)}.gz`;
    const mtime = parseInt(fs.statSync(inputFile).mtimeMs / 1000);
    return `  {"/${destination}", v${i}, sizeof(v${i}) - 1, ${mtime}},  // size: ${zipped.length}`;
  }).join('\n');

  return `// DO NOT EDIT. This file is generated using this command:
// ${process.argv.join(' ')}

#include "mongoose.h"

${decls}

const struct mg_mem_file mg_packed_files[] = {
${entries}
  {NULL, NULL, 0, 0}
};
`;
}

Promise.all(inputFiles.map(inputFile => {
  const baseDir = path.dirname(path.resolve(inputFile));
  const html = fs.readFileSync(inputFile, 'utf8');
  return processHtml(html, baseDir).then(data => ({inputFile, data}));
}))
  .then(files => {
    const c = generateC(files);
    if (outputFile) {
      fs.writeFileSync(outputFile, c);
    } else {
      process.stdout.write(c);
    }
  })
  .catch(err => {
    console.error('Error:', err.message);
    process.exit(1);
  });
