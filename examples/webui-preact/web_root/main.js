'use strict';
import { h, html, render, useEffect, useState } from './preact.min.js';


const Configuration = function (props) {
  const [url, setUrl] = useState(props.config.url || '');
  const [pub, setPub] = useState(props.config.pub || '');
  const [sub, setSub] = useState(props.config.sub || '');

  useEffect(() => {
    setUrl(props.config.url);
    setPub(props.config.pub);
    setSub(props.config.sub);
  }, [props.config]);

  const update = (name, val) =>
    fetch('/api/config/set', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify({
        [name]: val
      })
    })
      .catch(err => {
        console.log(err);
        enable(false);
      });

  const updateurl = ev => update('url', url);
  const updatepub = ev => update('pub', pub);
  const updatesub = ev => update('sub', sub);

  return html`
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
</div>`;
};


const App = function (props) {
  const [config, setConfig] = useState({});

  const getconfig = () =>
    fetch('/api/config/get')
      .then(r => r.json())
      .then(r => setConfig(r))
      .catch(err => console.log(err));

  useEffect(() => {
    getconfig();
  }, []);

  return html`
<h1>Basic Preact-based UI demo</h1>
<div class="col col-6">
  ${h(Configuration, { config })}
</div>
<button onclick=${getconfig}>Get configuration</button>`;
};

window.onload = () => render(h(App), document.body);
