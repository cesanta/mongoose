'use strict';
import {Component, h, html, render, useEffect, useState} from './preact.min.js';

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

  This device dashboard is developed with modern and compact Preact framework,
  in order to fit on a very small devices. This is
  a <a href="https://mongoose.ws/tutorials/http-server/">hybrid server</a> which
  provides both static and dynamic content.  Static files, like CSS/JS/HTML
  or images, are compiled into the server binary. Dynamic content is
  served via the REST API. 

  This dashboard shows values kept in server memory. Many clients can open
  this page.  The JS code that watches state changes, reconnects on network
  failures.  That means if server restarts, dashboard on all connected clients
  refresh automatically.

  NOTE: administrators can change settings values, whilst users cannot.

  <p>
  This UI uses the REST API implemented by the server, which you can examine
  using  <code>curl</code> command-line utility:
  </p>

  <div><code>curl localhost:8000/api/config/get</code> - get current device configuration</div>
  <div><code>curl localhost:8000/api/config/set -d 'value1=7&value2=hello'</code> - set device configuration</div>
  <div><code>curl localhost:8000/api/message/send -d 'msg=hello'</code> - send chat message</div>
  <div><code>curl localhost:8000/api/watch</code> - get notifications on config changes or chat messages</div>

</div>`;

const ShowSettings = function(props) {
  return html`
<div style="margin: 0 0.3em;">
  <h3 style="background: #59d; color: #fff;padding: 0.4em;">Device configuration</h3>
  <div style="margin: 0.5em 0;">
    <span class="addon">value1:</span>
    <input disabled type="text" class="smooth"
      value=${(props.config || {}).value1 || ''} />
  </div>
  <div style="margin: 0.5em 0;">
    <span class="addon">value2:</span>
    <input disabled type="text" class="smooth" 
      value=${(props.config || {}).value2 || ''} />
  </div>
  <div class="msg">
    Server's corresponding runtime configuration structure:
    <pre>
struct config {
  int value1;
  char *value2;
} s_config;
    </pre>
  </div>
</div>`;
};

const ChangeSettings = function(props) {
  const [value1, setValue1] = useState('');
  const [value2, setValue2] = useState('');
  useEffect(() => {
    setValue1(props.config.value1);
    setValue2(props.config.value2);
  }, [props.config.value1, props.config.value2])
  const update = (name, val) => fetch('/api/config/set', {
                                  method: 'post',
                                  body: `${name}=${encodeURIComponent(val)}`
                                }).catch(err => err);
  const updatevalue1 = ev => update('value1', value1);
  const updatevalue2 = ev => update('value2', value2);
  return html`
<div style="margin: 0 0.3em;">
  <h3 style="background: #c03434; color: #fff; padding: 0.4em;">
    Change configuration</h3>
  <div style="margin: 0.5em 0;">
    <span class="addon">value1:</span>
    <input type="text" value=${value1} onchange=${updatevalue1}
          oninput=${ev => setValue1(ev.target.value)} />
    <button class="btn" disabled=${!value1} onclick=${updatevalue1}
      style="margin-left: 1em; background: #8aa;">Update</button>
  </div>
  <div style="margin: 0.5em 0;">
    <span class="addon">value2:</span>
    <input type="text" value=${value2} onchange=${updatevalue2}
          oninput=${ev => setValue2(ev.target.value)} />
    <button class="btn" disabled=${!value2} onclick=${updatevalue2}
      style="margin-left: 1em; background: #8aa;">Update</button>
  </div>
  <div class="msg">
    As soon as administrator updates configuration, 
    server iterates over all connected clients and sends update
    notifications to all of them - so they update automatically.
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

const Chat = function(props) {
  const [message, setMessage] = useState('');
  const sendmessage = ev => fetch('/api/message/send', {
                              method: 'post',
                              body: `message=${encodeURIComponent(message)}`
                            }).then(r => setMessage(''));

  const messages = props.messages.map(
      text => html`<div style="margin: 0.5em 0;">${text}</div>`);
  return html`
<div style="margin: 0 0.3em;">
  <h3 style="background: #30c040; color: #fff; padding: 0.4em;">User chat</h3>
  <div style="height: 10em; overflow: auto; padding: 0.5em; " class="border">
    ${messages}
  </div>
  <div style="margin: 0.5em 0;">
    <input placeholder="type message..." style="width: 100%" value=${message}
      onchange=${sendmessage}
      oninput=${ev => setMessage(ev.target.value)} />
  </div>
  <div class="msg">
    Chat demonsrates
    real-time bidirectional data exchange between many clients and a server.
  </div>
</div>`;
};

const App = function(props) {
  const [messages, setMessages] = useState([]);
  const [user, setUser] = useState('');
  const [config, setConfig] = useState({});

  const refresh = () =>
      fetch('/api/config/get').then(r => r.json()).then(r => setConfig(r));

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
        var notification = JSON.parse(data);
        if (notification.name == 'config') refresh();
        if (notification.name == 'message') {
          setMessages(m => m.concat([notification.data]))
        }
        // console.log(notification);
        if (!result.done) return f(reader);
      });
    };
    fetch('/api/watch')
        .then(r => r.body.getReader())
        .then(f)
        .catch(e => setTimeout(watch, 1000));
  };

  useEffect(() => {
    fetch('/api/login')
        .then(r => r.json())
        .then(r => login(r))
        .catch(err => setUser(''));
    refresh();
    watch();
  }, []);

  if (!user) return html`<${Login} login=${login} />`;
  const admin = user == 'admin';
  const colsize = admin ? 'c4' : 'c6';
  const cs = admin ? html`<${ChangeSettings} config=${config} />` : '';
  return html`
<${Nav} user=${user} logout=${logout} />
<div class="container">
  <${Hero} />
  <div class="row">
    <div class="col ${colsize}"><${ShowSettings} config=${config} /></div>
    <div class="col ${colsize}">${cs}</div>
    <div class="col ${colsize}"><${Chat} messages=${messages} /></div>
  </div>
  <${Footer} />
</div>`;
};

window.onload = () => render(h(App), document.body);
