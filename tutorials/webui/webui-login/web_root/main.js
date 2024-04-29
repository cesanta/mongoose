'use strict';
import { h, html, render, useEffect, useState } from './preact.min.js';

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

const Main = props => html`
<div class="row">
  <H1>${props.data.text}</H1>
</div>`;

const Login = function (props) {
  const [user, setUser] = useState('');
  const [pass, setPass] = useState('');
  const login = () =>
    fetch(
      '/api/login',
      { headers: { Authorization: 'Basic ' + btoa(user + ':' + pass) } })
      .then(r => r.json())
      .then(r => r && props.login(r))
      .catch(err => err);
  return html`
<div class="rounded border" style="max-width: 480px; margin: 0 auto; margin-top: 5em; background: #eee; ">
  <div style="padding: 2em; ">
    <h1 style="color: #666;">Your Product Login </h1>
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



const App = function () {
  const [user, setUser] = useState('');
  const [data, setData] = useState({});

  const getin = () =>
    fetch('/api/data', { headers: { Authorization: '' } })
      .then(r => r.json())
      .then(r => setData(r))
      .catch(err => console.log(err));

  const login = function (u) {
    document.cookie = `access_token=${u.token}; Secure, HttpOnly; SameSite=Lax; path=/; max-age=3600`;
    setUser(u.user);
    return getin();
  };

  const logout = () => {
    document.cookie = `access_token=; Secure, HttpOnly; SameSite=Lax; path=/; max-age=0`;
    setUser('');
  };

  useEffect(() => {
    // Called once at init time
    fetch('/api/login', { headers: { Authorization: '' } })
      .then(r => r.json())
      .then(r => login(r))
      .catch(() => setUser(''));
  }, []);

  if (!user) return html`<${Login} login=${login} />`;

  return html`
<${Nav} user=${user} logout=${logout} />
<${Main} data=${data} />
`;
};

window.onload = () => render(h(App), document.body);
