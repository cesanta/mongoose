'use strict';
import {Component, h, html, render, useEffect, useState, useRef} from './preact.min.js';

const MaxMetricsDataPoints = 50;

// This simple publish/subscribe is used to pass notifications that were
// received from the server, to all child components of the app.
var PubSub = (function() {
  var handlers = {}, id = 0;
  return {
    subscribe: function(fn) {
      handlers[id++] = fn;
    },
    unsubscribe: function(id) {
      delete handlers[id];
    },
    publish: function(data) {
      for (var k in handlers) handlers[k](data);
    }
  };
})();

const Nav = props => html`
<div style="background: #333; padding: 0.5em; color: #fff;">
  <div class="container d-flex">
    <div style="flex: 1 1 auto; display: flex; align-items: center;">
      <b>Your Product</b>
    </div>
    <div style="display: flex; align-items: center; flex: 0 0 auto; ">
      <span>Logged in as:</span>
      <span style="padding: 0 0.5em;"><img src="user.png" height="22" /></span>
      <span>${props.user}</span>
      <a class="btn" onclick=${props.logout}
        style="margin-left: 1em; font-size: 0.8em; background: #8aa;">logout</a>
    </div>
  </div>
</div>`;


const Hero = props => html`
<div class="section">
<div style="margin-top: 1em; background: #eee; padding: 1em; border-radius: 0.5em; color: #777; ">
  <h1 style="margin: 0.2em 0;">Interactive Device Dashboard</h1>

  <p>
  This device dashboard is developed with modern and compact Preact framework,
  in order to fit on a very small devices. This is
  a <a href="https://mongoose.ws/tutorials/http-server/">hybrid server</a> which
  provides both static and dynamic content.  Static files, like CSS/JS/HTML
  or images, are compiled into the server binary.

  This UI uses the REST API implemented by the device, which you can examine
  using  <code>curl</code> command-line utility:
  </p>

  <div><code>curl localhost:8000/api/config/get</code> </div>
  <div><code>curl localhost:8000/api/config/set -d 'value1=7&value2=hello'</code> </div>
  <div><code>curl localhost:8000/api/message/send -d 'msg=hello'</code> </div>

  <p>
  A device can send notifications to this dashboard at anytime. Notifications
  are sent over Websocket at URI <code>curl localhost:8000/api/watch</code>
  as JSON strings <code>{"name": "..", "data": ...}</code>
  </p>
</div>
</div>`;

const Login = function(props) {
  const [user, setUser] = useState('');
  const [pass, setPass] = useState('');
  const login = ev =>
      fetch(
          '/api/login',
          {headers: {Authorization: 'Basic ' + btoa(user + ':' + pass)}})
          .then(r => r.json())
          .then(r => r && props.login(r))
          .catch(err => err);
  return html`
<div class="rounded border" style="max-width: 480px; margin: 0 auto; margin-top: 5em; background: #eee; ">
  <div style="padding: 2em; ">
    <h1 style="color: #666;">Device Dashboard Login </h1>
    <div style="margin: 0.5em 0;">
      <input type='text' placeholder='Name' style="width: 100%;"
        oninput=${ev => setUser(ev.target.value)} value=${user} />
    </div>
    <div style="margin: 0.5em 0;">
      <input type="password" placeholder="Password" style="width: 100%;"
        oninput=${ev => setPass(ev.target.value)} value=${pass}
        onchange=${login} />
    </div>
    <div style="margin: 1em 0;">
      <button class="btn" style="width: 100%; background: #8aa;"
        disabled=${!user || !pass} onclick=${login}> Login </button>
    </div>
    <div style="color: #777; margin-top: 2em;">
      Valid logins: admin:pass0, user1:pass1, user2:pass2
    </div>
  </div>
</div>`;
};


const Configuration = function(props) {
  const [url, setUrl] = useState(props.config.url || '');
  const [pub, setPub] = useState(props.config.pub || '');
  const [sub, setSub] = useState(props.config.sub || '');

  useEffect(() => {
    setUrl(props.config.url);
    setPub(props.config.pub);
    setSub(props.config.sub);
  }, [props.config]);

  const update = (name, val) => fetch('/api/config/set', {
                                  method: 'post',
                                  body: `${name}=${encodeURIComponent(val)}`
                                }).catch(err => err);
  const updateurl = ev => update('url', url);
  const updatepub = ev => update('pub', pub);
  const updatesub = ev => update('sub', sub);

  console.log(props, [url, pub, sub]);
  return html`
<div class="section">
  <h3 style="background: #c03434; color: #fff; padding: 0.4em;">
    Device Configuration</h3>
  <div style="margin: 0.5em 0; display: flex;">
    <span class="addon nowrap">MQTT server:</span>
    <input type="text" style="flex: 1 100%;"
          value=${url} onchange=${updateurl}
          oninput=${ev => setUrl(ev.target.value)} />
    <button class="btn" disabled=${!url} onclick=${updateurl}
      style="margin-left: 1em; background: #8aa;">Update</button>
  </div>
  <div style="margin: 0.5em 0; display: flex; ">
    <span class="addon nowrap">Subscribe topic:</span>
    <input type="text" style="flex: 1 100%;"
        value=${sub} onchange=${updatesub}
        oninput=${ev => setSub(ev.target.value)} />
    <button class="btn" disabled=${!sub} onclick=${updatesub}
      style="margin-left: 1em; background: #8aa;">Update</button>
  </div>
  <div style="margin: 0.5em 0; display: flex;">
    <span class="addon nowrap">Publish topic:</span>
    <input type="text" style="flex: 1 100%;"
          value=${pub} onchange=${updatepub}
          oninput=${ev => setPub(ev.target.value)} />
    <button class="btn" disabled=${!pub} onclick=${updatepub}
      style="margin-left: 1em; background: #8aa;">Update</button>
  </div>
  <div>
    You can use <a href="http://www.hivemq.com/demos/websocket-client/">
    HiveMQ Websocket web client</a> to send messages to this console.
  </div>
  <div class="msg">
    Device keeps a persistent connection to the configured MQTT server.
    Changes to this configuration are propagated to all dashboards: try
    changing them in this dashboard and observe changes in the other opened
    dashboard.
  </div><div class="msg">
    Note: administrators can see this section and can change device
    configuration, whilst users cannot.
  </div>
</div>`;
};


const Message = m => html`<div style="margin: 0.5em 0;">
  <span class="qos">qos: ${m.message.qos} </span>
  <span class="topic">topic: ${m.message.topic} </span>
  <span class="data">data: ${m.message.data}</span>
</div>`;

const Messages = function(props) {
  const [messages, setMessages] = useState([]);
  const [txt, setTxt] = useState('');

  useEffect(() => {
    const id = PubSub.subscribe(function(msg) {
      if (msg.name == 'message') setMessages(x => x.concat([msg.data]));
    });
    return PubSub.unsubscribe(id);
  }, []);

  const sendmessage = ev => fetch('/api/message/send', {
                              method: 'post',
                              body: `message=${encodeURIComponent(txt)}`
                            }).then(r => setTxt(''));
  const connstatus = props.config.connected ? 'connected' : 'diconnected';
  return html`
<div class="section">
  <h3 style="background: #30c040; color: #fff; padding: 0.4em;">MQTT messages</h3>
  <div>
    MQTT server status: <b>${connstatus}</b>
  </div>
  <div style="height: 10em; overflow: auto; padding: 0.5em; " class="border">
    ${messages.map(message => h(Message, {message}))}
  </div>
  <div style="margin: 0.5em 0; display: flex">
    <span class="addon nowrap">Publish message:</span>
    <input placeholder="type and press enter..." style="flex: 1 100%;"
      value=${txt} onchange=${sendmessage}
      oninput=${ev => setTxt(ev.target.value)} />
  </div>
  <div class="msg">
    Message gets passed to the device via REST. Then a device sends it to
    the MQTT server over MQTT. All MQTT messages on a subscribed topic
    received by a device, are propagated to this dashboard via the
    /api/watch.
  </div>
</div>`;
};

// Expected arguments:
// data: timeseries, e.g. [ [1654361352, 19], [1654361353, 18], ... ]
// width, height, yticks, xticks, ymin, ymax, xmin, xmax
const SVG = function(props) {
  //            w
  //   +---------------------+
  //   |        h1           |
  //   |    +-----------+    |
  //   |    |           |    |  h
  //   | w1 |           | w2 |
  //   |    +-----------+    |
  //   |         h2          |
  //   +---------------------+
  //
  let w = props.width, h = props.height, w1 = 30, w2 = 0, h1 = 8, h2 = 18;
  let yticks = props.yticks || 4, xticks = props.xticks || 5;
  let data = props.data || [];
  let ymin = props.ymin || 0;
  let ymax = props.ymax || Math.max.apply(null, data.map(p => p[1]));
  let xmin = props.xmin || Math.min.apply(null, data.map(p => p[0]));
  let xmax = props.xmax || Math.max.apply(null, data.map(p => p[0]));

  // Y-axis tick lines and labels
  let yta = (new Array(yticks + 1)).fill(0).map((_, i) => i);  // indices
  let yti = i => h - h2 - (h - h1 - h2) * i / yticks;          // index's Y
  let ytv = i => (ymax - ymin) * i / yticks;
  let ytl = y => html`<line x1=${w1} y1=${y} x2=${w} y2=${y} class="tick"/>`;
  let ytt = (y, v) => html`<text x=0 y=${y + 5} class="label">${v}</text>`;

  // X-axis tick lines and labels
  let datefmt = unix => (new Date(unix * 1000)).toISOString().substr(14, 5);
  let xta = (new Array(xticks + 1)).fill(0).map((_, i) => i);  // indices
  let xti = i => w1 + (w - w1 - w2) * i / xticks;              // index's X
  let xtv = i => datefmt(xmin + (xmax - xmin) * i / xticks);
  let xtl = x => html`<path d="M ${x},${h1} L ${x},${h - h2}" class="tick"/>`;
  let xtt = (x, v) =>
      html`<text x=${x - 15} y=${h - 2} class="label">${v}</text>`;

  // Transform data points array into coordinate
  let dx = v => w1 + (v - xmin) / ((xmax - xmin) || 1) * (w - w1 - w2);
  let dy = v => h - h2 - (v - ymin) / ((ymax - ymin) || 1) * (h - h1 - h2);
  let dd = data.map(p => [Math.round(dx(p[0])), Math.round(dy(p[1]))]);
  let ddl = dd.length;
  // And plot the data as <path> element
  let begin0 = ddl ? `M ${dd[0][0]},${dd[0][1]}` : `M 0,0`;
  let begin = `M ${w1},${h - h2}`;  // Initial point
  let end = ddl ? `L ${dd[ddl - 1][0]},${h - h2}` : `L ${w1},${h - h2}`;
  let series = ddl ? dd.map(p => `L ${p[0]} ${p[1]}`) : [];

  return html`
<svg viewBox="0 0 ${w} ${h}">
  <style>
    .axis { stroke: #aaa; fill: none; }
    .label { stroke: #aaa; font-size: 13px; }
    .tick { stroke: #ccc; fill: none; stroke-dasharray: 5; }
    .seriesbg { stroke: none; fill: rgba(200,225,255, 0.25)}
    .series { stroke: #25a; fill: none; }
  </style>
  ${yta.map(i => ytl(yti(i)))}
  ${yta.map(i => ytt(yti(i), ytv(i)))}
  ${xta.map(i => xtl(xti(i)))}
  ${data.length ? xta.map(i => xtt(xti(i), xtv(i))) : ''}
  <path d="${begin} ${series.join(' ')} ${end}" class="seriesbg" />
  <path d="${begin0} ${series.join(' ')}" class="series" />
</svg>`;
};


const Chart = function(props) {
  const [data, setData] = useState([]);
  useEffect(() => {
    const id = PubSub.subscribe(function(msg) {
      if (msg.name != 'metrics') return;
      setData(x => x.concat([msg.data]).splice(-MaxMetricsDataPoints));
    });
    return PubSub.unsubscribe(id);
  }, []);

  let xmax = 0, missing = MaxMetricsDataPoints - data.length;
  if (missing > 0) xmax = Math.round(Date.now() / 1000) + missing;
  return html`
<div class="section">
  <h3 style="background: #ec3; color: #fff; padding: 0.4em;">Data Chart</h3>
  <div style="overflow: auto; padding: 0.5em;" class="">
    <${SVG} height=240 width=600 ymin=0 ymax=20 xmax=${xmax} data=${data} />
  </div>
  <div class="msg">
    This chart plots live sensor data, sent by a device via /api/watch.
  </div>
</div>`;
};

const App = function(props) {
  const [user, setUser] = useState('');
  const [config, setConfig] = useState({});

  const getconfig = () =>
      fetch('/api/config/get', {headers: {Authorization: ''}})
          .then(r => r.json())
          .then(r => setConfig(r))
          .catch(err => console.log(err));

  const login = function(u) {
    document.cookie = `access_token=${u.token};path=/;max-age=3600`;
    setUser(u.user);
    return getconfig();
  };

  const logout = ev => {
    document.cookie = `access_token=;path=/;max-age=0`;
    setUser('');
  };

  // Watch for notifications. As soon as a notification arrives, pass it on
  // to all subscribed components
  const watch = function() {
    var l = window.location, proto = l.protocol.replace('http', 'ws');
    var tid, wsURI = proto + '//' + l.host + '/api/watch'
    var reconnect = function() {
      var ws = new WebSocket(wsURI);
      ws.onopen = () => console.log('ws connected');
      ws.onmessage = function(ev) {
        try {
          var msg = JSON.parse(ev.data);
          PubSub.publish(msg);
          if (msg.name != 'metrics') console.log('ws->', msg);
        } catch (e) {
          console.log('Invalid ws frame:', ev.data);  // eslint-disable-line
        }
      };
      ws.onclose = function() {
        clearTimeout(tid);
        tid = setTimeout(reconnect, 1000);
        console.log('ws disconnected');
      };
    };
    reconnect();
  };

  useEffect(() => {
    // Called once at init time
    PubSub.subscribe(msg => msg.name == 'config' && getconfig());
    fetch('/api/login', {headers: {Authorization: ''}})
        .then(r => r.json())
        .then(r => login(r))
        .then(watch)
        .catch(err => setUser(''));
  }, []);

  if (!user) return html`<${Login} login=${login} />`;

  return html`
<${Nav} user=${user} logout=${logout} />
<div class="container">
  <div class="row">
    <div class="col col-6"><${Hero} /></div>
    <div class="col col-6"><${Chart} /></div>
    <div class="col col-6">
      ${user == 'admin' && h(Configuration, {config})}
    </div>
    <div class="col col-6"><${Messages} config=${config} /></div>
  </div>
</div>`;
};

window.onload = () => render(h(App), document.body);
