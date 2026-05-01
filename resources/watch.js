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
let ignoreEventsUntil = 0;
let lastStartAt = 0;
const MIN_RESTART_MS = 250;

function start() {
  // Prevent restart loops caused by the command touching watched paths.
  ignoreEventsUntil = Date.now() + 200;
  lastStartAt = Date.now();

  if (child) {
    child.kill('SIGTERM');
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
    // fs.watchFile is polling-based; use a small interval to react quickly.
    fs.watchFile(target, { interval: 100 }, (curr, prev) => {
      if (Date.now() < ignoreEventsUntil) return;
      if (curr.mtimeMs !== prev.mtimeMs) {
        console.log(`File changed: ${target}`);
        clearTimeout(restartTimeout);
        const delay = Math.max(25, MIN_RESTART_MS - (Date.now() - lastStartAt));
        restartTimeout = setTimeout(start, delay);
      }
    });
  } else {
    const dir = target;
    fs.watch(dir, { recursive: true }, (eventType, filename) => {
      if (Date.now() < ignoreEventsUntil) return;
      if (filename) {
        console.log(`File changed: ${filename}`);
        clearTimeout(restartTimeout);
        const delay = Math.max(25, MIN_RESTART_MS - (Date.now() - lastStartAt));
        restartTimeout = setTimeout(start, delay);
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
