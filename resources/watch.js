// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

const { spawn } = require('child_process');
const fs = require('fs');
const path = require('path');

const args = process.argv.slice(2);
const watchTargets = [];
let command = [];

for (let i = 0; i < args.length; i++) {
  if (args[i] === '-w' && i + 1 < args.length) {
    watchTargets.push(args[++i]);
  } else if (args[i] === '--') {
    command = args.slice(i + 1);
    break;
  } else if (!args[i].startsWith('-')) {
    command = args.slice(i);
    break;
  }
}

if (watchTargets.length === 0 || command.length === 0) {
  console.error('Usage: node watch.js -w DIR_OR_FILE [-w DIR_OR_FILE] -- COMMAND');
  process.exit(1);
}

let child = null;
let restartTimeout = null;

function start() {
  if (child) {
    child.kill();
  }
  console.log(`Starting: ${command.join(' ')}`);
  child = spawn(command[0], command.slice(1), { stdio: 'inherit', shell: true });
  child.on('exit', (code) => {
    if (code !== null && code !== 0) {
      console.log(`Process exited with code ${code}`);
    }
  });
}

function watch(target) {
  const stats = fs.statSync(target);
  if (stats.isFile()) {
    fs.watchFile(target, (curr, prev) => {
      if (curr.mtime !== prev.mtime) {
        console.log(`File changed: ${target}`);
        clearTimeout(restartTimeout);
        restartTimeout = setTimeout(start, 100);
      }
    });
  } else {
    const dir = target;
    fs.watch(dir, { recursive: true }, (eventType, filename) => {
      if (filename) {
        console.log(`File changed: ${filename}`);
        clearTimeout(restartTimeout);
        restartTimeout = setTimeout(start, 100);
      }
    });
  }
}

start();
watchTargets.forEach(target => watch(target));
console.log(`Watching ${watchTargets.join(', ')} for changes...`);

process.on('SIGINT', () => {
  console.log('\nShutting down...');
  if (child) {
    child.kill('SIGTERM');
  }
  process.exit(0);
});
