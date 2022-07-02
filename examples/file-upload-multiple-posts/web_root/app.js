// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

// Helper function to display upload status
var setStatus = function(text) {
  document.getElementById('el3').innerText = text;
};

// When user clicks on a button, trigger file selection dialog
var button = document.getElementById('el2');
button.onclick = function(ev) {
  input.click();
};

// Send a large blob of data chunk by chunk
var sendFileData = function(name, data, chunkSize) {
  var sendChunk = function(offset) {
    var chunk = data.subarray(offset, offset + chunkSize) || '';
    var opts = {method: 'POST', body: chunk};
    var url = '/upload?offset=' + offset + '&name=' + encodeURIComponent(name);
    var ok;
    setStatus(
        'Uploading ' + name + ', bytes ' + offset + '..' +
        (offset + chunk.length) + ' of ' + data.length);
    fetch(url, opts)
        .then(function(res) {
          if (res.ok && chunk.length > 0) sendChunk(offset + chunk.length);
          ok = res.ok;
          return res.text();
        })
        .then(function(text) {
          if (!ok) setStatus('Error: ' + text);
        });
  };
  sendChunk(0);
};

// If user selected a file, read it into memory and trigger sendFileData()
var input = document.getElementById('el1');
input.onchange = function(ev) {
  if (!ev.target.files[0]) return;
  var f = ev.target.files[0], r = new FileReader();
  r.readAsArrayBuffer(f);
  r.onload = function() {
    ev.target.value = '';
    sendFileData(f.name, new Uint8Array(r.result), 2048);
  };
};
