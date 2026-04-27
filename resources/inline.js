// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

const fs = require('fs');
const path = require('path');
const https = require('https');
const http = require('http');

const args = process.argv.slice(2);
let outputFile = null;
let inputFile = null;

for (let i = 0; i < args.length; i++) {
  if (args[i] === '-o' && i + 1 < args.length) {
    outputFile = args[++i];
  } else if (!args[i].startsWith('-')) {
    inputFile = args[i];
  }
}

if (!inputFile) {
  console.error('Usage: node inline.js [-o OUTPUT_FILE] HTML_FILE');
  process.exit(1);
}

const baseDir = path.dirname(path.resolve(inputFile));
const html = fs.readFileSync(inputFile, 'utf8');

function fetchUrl(url) {
  return new Promise((resolve, reject) => {
    const client = url.startsWith('https') ? https : http;
    client.get(url, res => {
      let data = '';
      res.on('data', chunk => data += chunk);
      res.on('end', () => resolve(data));
      res.on('error', reject);
    }).on('error', reject);
  });
}

async function inlineCss(match) {
  const href = match.match(/href=["']([^"']+)["']/)[1];
  let css = '';
  if (href.startsWith('http')) {
    css = await fetchUrl(href);
  } else {
    const cssPath = path.join(baseDir, href);
    css = fs.readFileSync(cssPath, 'utf8');
  }
  return `<style>${css}</style>`;
}

async function inlineJs(match) {
  const match2 = match.match(/src=["']([^"']+)["']/);
  if (!match2) return match;
  const src = match2[1];
  let js = '';
  if (src.startsWith('http')) {
    js = await fetchUrl(src);
  } else {
    const jsPath = path.join(baseDir, src);
    js = fs.readFileSync(jsPath, 'utf8');
  }
  return `<script>${js}</script>`;
}

async function inlineImg(match) {
  const match2 = match.match(/src=["']([^"']+)["']/);
  if (!match2) return match;
  const src = match2[1];
  const ext = path.extname(src).toLowerCase();
  let mime = 'image/png';
  if (ext === '.svg') mime = 'image/svg+xml';
  else if (ext === '.jpg' || ext === '.jpeg') mime = 'image/jpeg';
  else if (ext === '.gif') mime = 'image/gif';
  else if (ext === '.webp') mime = 'image/webp';
  else if (ext === '.ico') mime = 'image/x-icon';
  let data;
  if (src.startsWith('http')) {
    data = await fetchUrl(src);
  } else {
    const imgPath = path.join(baseDir, src);
    data = fs.readFileSync(imgPath);
  }
  const base64 = Buffer.from(data).toString('base64');
  if (ext === '.svg') {
    const svgData = typeof data === 'string' ? data : data.toString('utf8');
    return match.replace(/src="[^"]+"/, `src="data:${mime};utf8,${encodeURIComponent(svgData)}"`);
  }
  return match.replace(/src="[^"]+"/, `src="data:${mime};base64,${base64}"`);
}

async function inlineSvg(match) {
  const match2 = match.match(/href=["']([^"']+)["']/);
  if (!match2) return match;
  const href = match2[1];
  if (!href.toLowerCase().endsWith('.svg')) return match;
  let data;
  if (href.startsWith('http')) {
    data = await fetchUrl(href);
  } else {
    const svgPath = path.join(baseDir, href);
    data = fs.readFileSync(svgPath, 'utf8');
  }
  const base64 = Buffer.from(data).toString('base64');
  return match.replace(/href="[^"]+"/, `href="data:image/svg+xml;base64,${base64}"`);
}

async function processHtml(content) {
  let result = content;
  const cssRegex = /<link[^>]+rel=["']stylesheet["'][^>]*>/g;
  const jsRegex = /<script[^>]+src=["'][^"']+["'][^>]*><\/script>/g;
  const imgRegex = /<img[^>]+src=["'][^"']+["'][^>]*>/g;
  const svgRegex = /<link[^>]+rel=["']icon["'][^>]*>/g;

  const cssMatches = content.match(cssRegex) || [];
  for (const match of cssMatches) {
    if (!match.includes('data:')) {
      result = result.replace(match, await inlineCss(match));
    }
  }

  const jsMatches = content.match(jsRegex) || [];
  for (const match of jsMatches) {
    if (!match.includes('data:')) {
      result = result.replace(match, await inlineJs(match));
    }
  }

  const imgMatches = content.match(imgRegex) || [];
  for (const match of imgMatches) {
    if (!match.includes('data:')) {
      result = result.replace(match, await inlineImg(match));
    }
  }

  const svgMatches = content.match(svgRegex) || [];
  for (const match of svgMatches) {
    if (!match.includes('data:')) {
      result = result.replace(match, await inlineSvg(match));
    }
  }

  return result;
}

processHtml(html).then(result => {
  if (outputFile) {
    fs.writeFileSync(outputFile, result);
    console.log(`Written to ${outputFile}`);
  } else {
    console.log(result);
  }
}).catch(err => {
  console.error('Error:', err.message);
  process.exit(1);
});
