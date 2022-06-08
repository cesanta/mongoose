'use strict';
import {Component, h, html, render, useEffect, useState, useRef} from './preact.min.js';

const Message = m => html`<span
  style="color: ${m.message.uart ? '#444' : '#373'};">
  ${m.message.data}
</span>`;

const App = function(props) {
  const [cfg, setCfg] = useState({tcp: '', ws: ''});
  const [messages, setMessages] = useState([]);
  const [txt, setTxt] = useState('');
  const [connected, setConnected] = useState(false);
  const [ws, setWs] = useState(null);

  const tcp_port = cfg.tcp.split(':')[2] || 4001;
  const ws_port = cfg.ws.split(':')[2] || 4002;

  const refresh = () =>
      fetch('/api/config/get').then(r => r.json()).then(r => setCfg(r));

  const watchWebsocket = function() {
    // Connect to websocker port, to implement WS console
    var l = window.location, proto = l.protocol.replace('http', 'ws');
    var tid, url = `${proto}//${l.hostname}:${ws_port}/ws`;
    console.log(url);
    var reconnect = function() {
      var ws = new WebSocket(url);
      ws.onopen = () => {
        setConnected(true);
        setWs(ws);
      };
      ws.onmessage = ev => {
        // console.log(ev, ev.data);
        setMessages(x => x.concat([{data: ev.data, uart: true}]));
      };
      ws.onclose = function() {
        clearTimeout(tid);
        tid = setTimeout(reconnect, 1000);
        setConnected(false);
        setWs(null);
      };
    };
    reconnect();
  };

  useEffect(() => {
    refresh();
    watchWebsocket();
  }, []);


  const sendmessage = ev => {
    setMessages(x => x.concat([{data: txt + '\n', uart: false}]));
    if (ws) ws.send(txt + '\n');
    setTxt('');
  };

  return html`
<div class="container">
  <h1>UART \u27F7 network bridge </h1>
  <h3>UART settings</h3>
  <pre class="d-none">${JSON.stringify(cfg, null, 2)}</pre>
  <p>RX pin ${cfg.rx}, TX pin ${cfg.tx}, baud ${cfg.baud}</p>
  <h3>TCP listener: port ${tcp_port}</h3>
  <p>Use netcat to connect:</p>
  <div class="msg"> $ nc ${location.hostname} ${tcp_port} </div>
  <h3>Websocket listener: port ${ws_port}</h3>
  <div>UART console. Status: ${connected ? 'connected' : 'disconnected'}</div>
  <pre style="height: 15em; overflow: auto;">
    ${messages.map(message => h(Message, {message}))}
  </pre>
  <div style="margin: 0.5em 0; display: flex">
    <input placeholder="type and press enter..." style="flex: 1 100%;"
      value=${txt} onchange=${sendmessage}
      oninput=${ev => setTxt(ev.target.value)} />
  </div>
</div>`;
};

window.onload = () => render(h(App), document.body);
