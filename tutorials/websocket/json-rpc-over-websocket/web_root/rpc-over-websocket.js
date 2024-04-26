// JSON-RPC over Websocket implementation
var JSONRPC_TIMEOUT_MS = 1000;

var jsonrpc = function(url, onopen, onclose, onnotification) {
  var rpcid = 0, pending = {}, ws = new WebSocket(url);
  if (!ws) return null;
  ws.onclose = onclose;
  ws.onmessage = function(ev) {
    const frame = JSON.parse(ev.data);
    console.log('rcvd', frame, 'pending:', pending);
    if (frame.id !== undefined) {
      if (pending[frame.id] !== undefined) pending[frame.id](frame);  // Resolve
      delete (pending[frame.id]);
    } else {
      if (onnotification) onnotification(frame);
    }
  };
  if (onopen) onopen();
  return {
    close: () => ws.close(),
    call: function(method, params) {
      const id = rpcid++, request = {id, method, params};
      ws.send(JSON.stringify(request));
      console.log('sent', request);
      return new Promise(function(resolve, reject) {
        setTimeout(function() {
          if (pending[id] === undefined) return;
          log('Timing out frame ', JSON.stringify(request));
          delete (pending[id]);
          reject();
        }, JSONRPC_TIMEOUT_MS);
        pending[id] = x => resolve(x);
      });
    },
  };
};
