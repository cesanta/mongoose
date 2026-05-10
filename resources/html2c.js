// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
//
// Generate a Mongoose packed filesystem C file from an HTML file after
// inlining referenced CSS, JS, images, and icons.
//
// Usage:
//   node html2c.js [-o OUTPUT.c] HTML_FILE

const fs = require('fs');
const path = require('path');
const http = require('http');
const https = require('https');

const args = process.argv.slice(2);
let outputFile = null;
let inputFile = null;

for (let i = 0; i < args.length; i++) {
  if (args[i] === '-o' && i + 1 < args.length) {
    outputFile = args[++i];
  } else if (!args[i].startsWith('-') && inputFile === null) {
    inputFile = args[i];
  }
}

if (!inputFile) {
  console.error('Usage: node html2c.js [-o OUTPUT.c] HTML_FILE');
  process.exit(1);
}

const baseDir = path.dirname(path.resolve(inputFile));
const html = fs.readFileSync(inputFile, 'utf8');

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

async function readAsset(name, encoding) {
  if (name.startsWith('http://') || name.startsWith('https://')) {
    const data = await fetchUrl(name);
    return encoding ? data.toString(encoding) : data;
  }
  return fs.readFileSync(path.join(baseDir, name), encoding || null);
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

async function inlineCss(match) {
  const href = attr(match, 'href');
  if (!href) return match;
  const css = await readAsset(href, 'utf8');
  return `<style>${css}</style>`;
}

async function inlineJs(match) {
  const src = attr(match, 'src');
  if (!src) return match;
  const js = await readAsset(src, 'utf8');
  return `<script>${js}</script>`;
}

async function inlineImg(match) {
  const src = attr(match, 'src');
  if (!src) return match;
  const ext = path.extname(src).toLowerCase();
  let mime = 'image/png';
  if (ext === '.svg') mime = 'image/svg+xml';
  else if (ext === '.jpg' || ext === '.jpeg') mime = 'image/jpeg';
  else if (ext === '.gif') mime = 'image/gif';
  else if (ext === '.webp') mime = 'image/webp';
  else if (ext === '.ico') mime = 'image/x-icon';

  const data = await readAsset(src);
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

async function inlineSvg(match) {
  const href = attr(match, 'href');
  if (!href || !href.toLowerCase().endsWith('.svg')) return match;
  const data = await readAsset(href);
  return replaceAttr(
    match,
    'href',
    `data:image/svg+xml;base64,${Buffer.from(data).toString('base64')}`,
  );
}

async function processHtml(content) {
  let result = content;
  const cssRegex = /<link[^>]+rel=["']stylesheet["'][^>]*>/g;
  const jsRegex = /<script[^>]+src=["'][^"']+["'][^>]*><\/script>/g;
  const imgRegex = /<img[^>]+src=["'][^"']+["'][^>]*>/g;
  const svgRegex = /<link[^>]+rel=["']icon["'][^>]*>/g;

  for (const match of content.match(cssRegex) || []) {
    if (!match.includes('data:')) result = result.replace(match, await inlineCss(match));
  }
  for (const match of content.match(jsRegex) || []) {
    if (!match.includes('data:')) result = result.replace(match, await inlineJs(match));
  }
  for (const match of content.match(imgRegex) || []) {
    if (!match.includes('data:')) result = result.replace(match, await inlineImg(match));
  }
  for (const match of content.match(svgRegex) || []) {
    if (!match.includes('data:')) result = result.replace(match, await inlineSvg(match));
  }

  return result;
}

function cArray(data) {
  return Array.from(Buffer.from(data, 'utf8')).concat(0).join(',');
}

function generateC(data) {
  const destination = path.basename(inputFile);
  const stat = fs.statSync(inputFile);
  const mtime = parseInt(stat.mtimeMs / 1000);
  const bytes = cArray(data);

  return `// DO NOT EDIT. This file is generated using this command:
// ${process.argv.join(' ')}

#include "mongoose.h"

static const unsigned char v0[] = {${bytes}};

const struct mg_mem_file mg_packed_files[] = {
  {"/${destination}", v0, sizeof(v0) - 1, ${mtime}},
  {NULL, NULL, 0, 0}
};
`;
}

processHtml(html)
  .then(result => {
    const c = generateC(result);
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
