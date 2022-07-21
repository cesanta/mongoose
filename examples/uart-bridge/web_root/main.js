'use strict';
import {Component, h, html, render, useEffect, useState, useRef} from './preact.min.js';

const Message = m => html`<span
  style="color: ${m.message.uart ? '#444' : '#373'};">
  ${m.message.data}
</span>`;

const App = function(props) {
  const [cfg, setCfg] = useState({tcp: {}, ws: {}, mqtt: {}});
  const [messages, setMessages] = useState([]);
  const [connected, setConnected] = useState(false);
  const [txt, setTxt] = useState('');
  const [ws, setWs] = useState(null);

  const refresh = () =>
      fetch('/api/config/get').then(r => r.json()).then(r => setCfg(r));

  const getport = (url, v) => ((url || '').match(/.*:(\d+)/) || ['', v])[1];

  const watchWebsocket = function() {
    // Connect to websocker port, to implement WS console
    var reconnect = function() {
      var port = getport(cfg.ws.url, 4002);
      var l = window.location, proto = l.protocol.replace('http', 'ws');
      var tid, url = `${proto}//${l.hostname}:${port}/ws`;
      // console.log(url);
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

  const onchange = ev => fetch('/api/config/set', {
                           method: 'POST',
                           headers: {'Content-Type': 'application/json'},
                           body: JSON.stringify(cfg),
                         }).then(r => ws && ws.close());

  const set = obj => setCfg(x => Object.assign(x, obj));
  const nset = (n,obj) => setCfg(x => Object.assign(x, {[n]: Object.assign(x[n],obj)}));
  const setTx = ev => set({tx: parseInt(ev.target.value)});
  const setRx = ev => set({rx: parseInt(ev.target.value)});
  const setBaud = ev => set({baud: parseInt(ev.target.value)});
  const setTcpUrl = ev => nset('tcp', {url: `tcp://0.0.0.0:${ev.target.value}`});
  const setWsUrl = ev => nset('ws',{url: `ws://0.0.0.0:${ev.target.value}`});
  const setMqttUrl = ev => nset('mqtt',{url: ev.target.value});
  const setTcpEna = ev => (nset('tcp',{enable: ev.target.checked}), onchange());
  const setWsEna = ev => (nset('ws',{enable: ev.target.checked}), onchange());
  const setMqttEna = ev =>(nset('mqtt', {enable: ev.target.checked}), onchange());

  return html`
<div class="container">
  <h1 style="margin-bottom: 0;">UART \u27F7 network bridge </h1>
  <pre class="d-none">${JSON.stringify(cfg, null, 2)}</pre>
  <div class="row">
    <div class="col col-4">
      <h3>UART configuration</h3>
      <div class="d-flex pr-1 my-1">
        <label class="addon">UART TX pin</label>
        <input style="width: 5em;" value=${cfg.tx} onchange=${onchange}
          oninput=${setTx} />
      </div><div class="d-flex pr-1 my-1">
        <label class="addon">UART RX pin</label>
        <input style="width: 5em;" value=${cfg.rx} onchange=${onchange}
          oninput=${setRx} />
      </div><div class="d-flex pr-1 my-y">
        <label class="addon">UART Baud</label>
        <input style="width: 5em;" value=${cfg.baud} onchange=${onchange}
          oninput=${setBaud} />
      </div>
    </div>
    <div class="col col-8">
      <h3>Network configuration</h3>
      <div class="d-flex my-1">
        <label class="addon">Local TCP port</label>
        <input style="min-width: 4em; flex: 1 100%;"
          value=${getport(cfg.tcp.url, 4001)} onchange=${onchange}
          oninput=${setTcpUrl} />
        <label class="ml-1 d-flex label"><input type="checkbox"
          checked=${cfg.tcp.enable} onchange=${setTcpEna} /> enable</label>
      </div><div class="d-flex my-1">
        <label class="addon">Local WS port</label>
        <input style="flex: 1 100%;"
          value=${getport(cfg.ws.url, 4002)} onchange=${onchange}
          oninput=${setWsUrl} />
        <label class="ml-1 d-flex label"><input type="checkbox"
          checked=${cfg.ws.enable} onchange=${setWsEna} /> enable</label>
      </div><div class="d-flex my-1">
        <label class="addon">Remote MQTT</label>
        <input style="flex: 1 100%;"
          value=${cfg.mqtt.url} onchange=${onchange}
          oninput=${setMqttUrl} />
        <label class="ml-1 d-flex label"><input type="checkbox"
          checked=${cfg.mqtt.enable} onchange=${setMqttEna} /> enable</label>
      </div>
    </div>
  </div>

  <div class="msg">
    Note: to connect over MQTT, 
      open <a href="http://www.hivemq.com/demos/websocket-client/">
        console</a>, subscribe to b/tx and publish to b/rx<br/>
    Note: to connect over TCP, use netcat utility:<br/>
    $ nc ${location.hostname} ${getport(cfg.tcp.url, 4001)}
  </div>

  <div style="margin-top: 2em;">
    <b>UART console</b><span style="margin-left: 1em; color: #777;">works 
    over the local WS port. WebSocket status: </span><span
      style="color: ${connected ? 'teal' : 'red'};">
      \u25cf ${connected ? 'connected' : 'disconnected'} </span>
  </div>
  <div style="margin: 0.5em 0; display: flex">
    <input placeholder="to send data, type and press enter..." style="flex: 1 100%;"
      value=${txt} onchange=${sendmessage}
      oninput=${ev => setTxt(ev.target.value)} />
    <button style="margin-left: 1em;"
      onclick=${ev => setMessages([])}>clear</button>
  </div>
  <pre style="height: 15em; overflow: auto;">
    ${messages.map(message => h(Message, {message}))}
  </pre>

</div>`;
};

window.onload = () => render(h(App), document.body);
