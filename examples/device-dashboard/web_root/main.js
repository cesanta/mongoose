'use strict';
import {Component, h, html, render, useEffect, useState, useRef} from './preact.min.js';

const MaxMetricsDataPoints = 50;

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

const Footer = props => html`
<div style="color: silver; margin-top: 2em; padding-top: 0.5em; border-top: 1px solid #ccc; ">
  Copyright (c) Your Company
</div>
`;


const Hero = props => html`
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

  <div><code>curl localhost:8000/api/config/get</code> - get current device configuration</div>
  <div><code>curl localhost:8000/api/config/set -d 'value1=7&value2=hello'</code> - set device configuration</div>
  <div><code>curl localhost:8000/api/message/send -d 'msg=hello'</code> - send MQTT message</div>
  <div><code>curl localhost:8000/api/watch</code> - get notifications: MQTT messages, configuration, sensor data</div>

</div>`;


const Configuration = function(props) {
  const [url, setUrl] = useState('');
  const [pub, setPub] = useState('');
  const [sub, setSub] = useState('');
  useEffect(() => {
    setUrl(props.config.url);
    setPub(props.config.pub);
    setSub(props.config.sub);
  }, [props.config.url, props.config.pub, props.config.sub])
  const update = (name, val) => fetch('/api/config/set', {
                                  method: 'post',
                                  body: `${name}=${encodeURIComponent(val)}`
                                }).catch(err => err);
  const updateurl = ev => update('url', url);
  const updatepub = ev => update('pub', pub);
  const updatesub = ev => update('sub', sub);
  return html`
<div style="margin: 0 0.3em;">
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

const Message = text => html`<div style="margin: 0.5em 0;">${text}</div>`;

const Messages = function(props) {
  const [message, setMessage] = useState('');
  const sendmessage = ev => fetch('/api/message/send', {
                              method: 'post',
                              body: `message=${encodeURIComponent(message)}`
                            }).then(r => setMessage(''));

  const messages = props.messages.map(
      text => html`<div style="margin: 0.5em 0;">${text}</div>`);
  return html`
<div style="margin: 0 0.3em;">
  <h3 style="background: #30c040; color: #fff; padding: 0.4em;">MQTT messages</h3>
  <div style="height: 10em; overflow: auto; padding: 0.5em; " class="border">
    ${messages}
  </div>
  <div style="margin: 0.5em 0; display: flex">
    <span class="addon nowrap">Publish message:</span>
    <input placeholder="type and press enter..." style="flex: 1 100%;"
      value=${message} onchange=${sendmessage}
      oninput=${ev => setMessage(ev.target.value)} />
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
  let xmax = 0, missing = MaxMetricsDataPoints - props.metrics.length;
  if (missing > 0) xmax = Math.round(Date.now() / 1000) + missing;
  return html`
<div style="margin: 0 0.3em;">
  <h3 style="background: #ec3; color: #fff; padding: 0.4em;">Data Chart</h3>
  <div style="overflow: auto; padding: 0.5em;" class="">
    <${SVG} height=240 width=600 ymin=0 ymax=20
      xmax=${xmax} data=${props.metrics} />
  </div>
  <div class="msg">
    This chart plots live sensor data, sent by a device via /api/watch.
  </div>
</div>`;
};

const App = function(props) {
  const [messages, setMessages] = useState([]);
  const [metrics, setMetrics] = useState([]);
  const [user, setUser] = useState('');
  const [config, setConfig] = useState({});

  const refresh = () => fetch('/api/config/get', {headers: {Authorization: ''}})
                            .then(r => r.json())
                            .then(r => setConfig(r));

  const login = function(u) {
    document.cookie = `access_token=${u.token};path=/;max-age=3600`;
    setUser(u.user);
    refresh();
  };

  const logout = ev => {
    document.cookie = `access_token=;path=/;max-age=0`;
    setUser('');
  };

  const watch = function() {
    var f = function(reader) {
      return reader.read().then(function(result) {
        var data = String.fromCharCode.apply(null, result.value);
        var msg = JSON.parse(data);
        if (msg.name == 'config') {
          refresh();
        } else if (msg.name == 'message') {
          setMessages(m => m.concat([msg.data]));
        } else if (msg.name == 'metrics') {
          setMetrics(m => m.concat([msg.data]).splice(-MaxMetricsDataPoints));
        }
        // console.log(msg);
        if (!result.done) return f(reader);
      });
    };
    fetch('/api/watch', {headers: {Authorization: ''}})
        .then(r => r.body.getReader())
        .then(f)
        .catch(e => setTimeout(watch, 1000));
  };

  useEffect(() => {
    fetch('/api/login', {headers: {Authorization: ''}})
        .then(r => r.json())
        .then(r => login(r))
        .catch(err => setUser(''));
    refresh();
    watch();
  }, []);

  if (!user) return html`<${Login} login=${login} />`;
  const admin = user == 'admin';
  const cs = admin ? html`<${Configuration} config=${config} />` : '';
  return html`
<${Nav} user=${user} logout=${logout} />
<div class="container">
  <${Hero} />
  <div class="row">
    <div class="col c6"><${Chart} metrics=${metrics} /></div>
    <div class="col c6">${cs}</div>
    <div class="col c6"><${Messages} messages=${messages} /></div>
  </div>
  <${Footer} />
</div>`;
};

window.onload = () => render(h(App), document.body);
