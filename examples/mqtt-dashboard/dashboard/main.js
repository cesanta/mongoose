'use strict';
import {h, html, render, useEffect, useRef, useState} from './bundle.js';

const Logo = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" viewBox="0 0 12.87 12.85"><defs><style>.ll-cls-1{fill:none;stroke:#000;stroke-miterlimit:10;stroke-width:0.5px;}</style></defs><g id="Layer_2" data-name="Layer 2"><g id="Layer_1-2" data-name="Layer 1"><path class="ll-cls-1" d="M12.62,1.82V8.91A1.58,1.58,0,0,1,11,10.48H4a1.44,1.44,0,0,1-1-.37A.69.69,0,0,1,2.84,10l-.1-.12a.81.81,0,0,1-.15-.48V5.57a.87.87,0,0,1,.86-.86H4.73V7.28a.86.86,0,0,0,.86.85H9.42a.85.85,0,0,0,.85-.85V3.45A.86.86,0,0,0,10.13,3,.76.76,0,0,0,10,2.84a.29.29,0,0,0-.12-.1,1.49,1.49,0,0,0-1-.37H2.39V1.82A1.57,1.57,0,0,1,4,.25H11A1.57,1.57,0,0,1,12.62,1.82Z"/><path class="ll-cls-1" d="M10.48,10.48V11A1.58,1.58,0,0,1,8.9,12.6H1.82A1.57,1.57,0,0,1,.25,11V3.94A1.57,1.57,0,0,1,1.82,2.37H8.9a1.49,1.49,0,0,1,1,.37l.12.1a.76.76,0,0,1,.11.14.86.86,0,0,1,.14.47V7.28a.85.85,0,0,1-.85.85H8.13V5.57a.86.86,0,0,0-.85-.86H3.45a.87.87,0,0,0-.86.86V9.4a.81.81,0,0,0,.15.48l.1.12a.69.69,0,0,0,.13.11,1.44,1.44,0,0,0,1,.37Z"/></g></g></svg>`;
const DefaultTopic = 'mg_mqtt_dashboard';
const DefaultUrl = location.protocol == 'https:'
  ? 'wss://broker.hivemq.com:8884/mqtt'
  : 'ws://broker.hivemq.com:8000/mqtt';
const DefaultDeviceConfig = {pin_map: [], pin_state: [], log_level: 0, pin_count: 0};
const Delay = (ms, val) => new Promise(resolve => setTimeout(resolve, ms, val));
const handleFetchError = r => r.ok || alert(`Error: ${r.statusText}`);
const LabelClass = 'text-sm truncate text-gray-500 font-medium my-auto whitespace-nowrap';
const BadgeClass = 'flex-inline text-sm rounded-md rounded px-2 py-0.5 ring-1 ring-inset';
const InputClass = 'font-normal text-sm rounded w-full flex-1 py-0.5 px-2 text-gray-700 placeholder:text-gray-400 focus:outline-none disabled:cursor-not-allowed disabled:bg-gray-100 disabled:text-gray-500 rounded border';
const TitleClass = 'font-semibold xuppercase xtext-gray-600';
const Colors = {
  green: 'bg-green-100 text-green-900 ring-green-300',
  yellow: 'bg-yellow-100 text-yellow-900 ring-yellow-300',
  info: 'bg-zinc-100 text-zinc-900 ring-zinc-300',
  red: 'bg-red-100 text-red-900 ring-red-300',
};

let MqttClient;

const Icons = {
  logo: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" width="59.1" height="75.7"  viewBox="0 0 1514 1182"> <path style="fill: #2f409a;" d="m52.8 572h345.1v346.5h-345.1zm-10.9-434.2c-12.6 6.4-32.2 20.9-35.5 37.9 0 0-6.1 35.9-6.1 36.8-2.1 13.1 10.5 10 15.8 6.7 53.9-31.8 113.9-60.7 168.7-77.5 258.2-78.2 585.9-80.2 643.1 88.8 40.3 117.1-123.6 214.8-216.1 258.4-35.7 16.8-47.6 32.8-50 49.8-2 15.3 3.2 37.8 23.8 68.3 129.7 194.8 515 474 893.1 571.7 15.7 4.2 23.5 2.8 29.9-1.1 3.5-2 7.9-4.8 1.1-8.2-251.1-132.2-490.9-353.9-610.9-525.1-27.4-39.5-34.5-65.9-32.7-87.2 1.7-18.4 18.7-34.5 54.8-51.4 134.1-62.3 270.9-157.1 230-314.9-59.5-226.9-700.5-260.1-1109-53z"/></svg>`,
  activity: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="24" height="24"  fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="22 12 18 12 15 21 9 3 6 12 2 12"></polyline></svg>`,
  refresh: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="24" height="24"  fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="23 4 23 10 17 10"></polyline><polyline points="1 20 1 14 7 14"></polyline><path d="M3.51 9a9 9 0 0 1 14.85-3.36L23 10M1 14l4.64 4.36A9 9 0 0 0 20.49 15"></path></svg>`,
  info: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="24" height="24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="10"></circle><line x1="12" y1="8" x2="12" y2="12"></line><line x1="12" y1="16" x2="12.01" y2="16"></line></svg>`,
  login: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M15 3h4a2 2 0 0 1 2 2v14a2 2 0 0 1-2 2h-4"></path><polyline points="10 17 15 12 10 7"></polyline><line x1="15" y1="12" x2="3" y2="12"></line></svg>`,
  logout: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M9 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h4"></path><polyline points="16 17 21 12 16 7"></polyline><line x1="21" y1="12" x2="9" y2="12"></line></svg>`,
  menu: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><line x1="3" y1="12" x2="21" y2="12"></line><line x1="3" y1="6" x2="21" y2="6"></line><line x1="3" y1="18" x2="21" y2="18"></line></svg>`,
  upload: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M21 15v4a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2v-4"></path><polyline points="17 8 12 3 7 8"></polyline><line x1="12" y1="3" x2="12" y2="15"></line></svg>`,
  monitor: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><rect x="2" y="3" width="20" height="14" rx="2" ry="2"></rect><line x1="8" y1="21" x2="16" y2="21"></line><line x1="12" y1="17" x2="12" y2="21"></line></svg>`,
  settings: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="3"></circle><path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z"></path></svg>`,
  download: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M21 15v4a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2v-4"></path><polyline points="7 10 12 15 17 10"></polyline><line x1="12" y1="15" x2="12" y2="3"></line></svg>`,
  file: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M13 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V9z"></path><polyline points="13 2 13 9 20 9"></polyline></svg>`,
  check: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="20 6 9 17 4 12"></polyline></svg>`,
  rollback: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="11 17 6 12 11 7"></polyline><polyline points="18 17 13 12 18 7"></polyline></svg>`,
  save: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M19 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h11l5 5v11a2 2 0 0 1-2 2z"></path><polyline points="17 21 17 13 7 13 7 21"></polyline><polyline points="7 3 7 8 15 8"></polyline></svg>`,
  bolt: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polygon points="13 2 3 14 12 14 11 22 21 10 12 10 13 2"></polygon></svg>`,
  delete: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><line x1="18" y1="6" x2="6" y2="18"></line><line x1="6" y1="6" x2="18" y2="18"></line></svg>`,
};

export function Button({title, onclick, disabled, extraClass, icon, ref, colors, hovercolor, disabledcolor}) {
  const [spin, setSpin] = useState(false);
  const cb = function(ev) {
    const res = onclick ? onclick() : null;
    if (res && typeof (res.catch) === 'function') {
      setSpin(true);
      res.catch(() => false).then(() => setSpin(false));
    }
  };
  if (!colors) colors = 'bg-blue-600 hover:bg-blue-500 disabled:bg-blue-400';
  return html`
<button type="button" class="inline-flex justify-center items-center gap-2 rounded px-2.5 py-0.5 text-sm font-semibold text-white shadow-sm ${colors} ${extraClass}"
  ref=${ref} onclick=${cb} disabled=${disabled || spin} >
  ${title}
  <${spin ? Icons.refresh : icon} class="w-4 ${spin ? 'animate-spin' : ''}" />
<//>`
};

function UploadFileButton({oncomplete, onprogress, onfileselect, url, title, colors, accept, extraClass}) {
  const btn = useRef(null);
  const input = useRef(null);

  // Send a large file chunk by chunk
  const sendFileData = function(url, fileName, fileData, chunkSize) {
    return new Promise(function(resolve, reject) {
      const finish = ok => {
        input.fn = null;
        const res = oncomplete ? oncomplete(ok, fileName, fileData.length) : null;
        if (res && typeof (res.catch) === 'function') {
          res.catch(() => false).then(() => ok ? resolve() : reject());
        } else {
          ok ? resolve() : reject();
        }
      };
      const sendChunk = function(offset) {
        var chunk = fileData.subarray(offset, offset + chunkSize) || '';
        var opts = {method: 'POST', body: chunk};
        var fullUrl = url + '?offset=' + offset +
          '&total=' + fileData.length  +
          '&file=' + encodeURIComponent(fileName);
        onprogress && onprogress(offset, fileData.length, fileName);
        fetch(fullUrl, opts)
          .then(function(res) {
            if (!res.ok) {
              finish(false);
            } else if (offset >= fileData.length) {
              finish(true);
            } else {
              sendChunk(offset + chunk.length);
            }
          });
      };
      sendChunk(0);
    });
  };

  const onchange = function(ev) {
    if (!ev.target.files[0]) return;
    let r = new FileReader(), f = ev.target.files[0];
    if (onfileselect && !onfileselect(f.name)) {
      ev.target.value = '';
      return;
    }
    r.readAsArrayBuffer(f);
    r.onload = function() {
      input.fn = sendFileData(url, f.name, new Uint8Array(r.result), 2048);
      ev.target.value = '';
      ev.preventDefault();
      btn.current && btn.current.base.click();
    };
  };

  const onclick = function(ev) {
    //if (!input.fn) input.current.click();  // No upload in progress, show file dialog
  };

  return html`
<div class="inline-flex flex-col ${extraClass}">
  <input class="hidden" type="file" ref=${input} onchange=${onchange} accept=${accept} />
  <${Button} title=${title} icon=${Icons.download} onclick=${onclick} ref=${btn} colors=${colors} />
<//>`;
};

function Toggle({value, onclick, disabled}) {
  const bg = !!value ? 'bg-blue-600' : 'bg-gray-200';
  const tr = !!value ? 'translate-x-5' : 'translate-x-0';
  return html`
<button type="button" onclick=${onclick} disabled=${disabled}
  class="${bg} inline-flex h-6 w-11 flex-shrink-0 cursor-pointer rounded-full border-2 border-transparent transition-colors duration-200 ease-in-out focus:outline-none focus:ring-0 ring-0"
  role="switch" aria-checked=${!!value}>
  <span aria-hidden="true" class="${tr} pointer-events-none inline-block h-5 w-5 transform rounded-full bg-white shadow ring-0 focus:ring-0 transition duration-200 ease-in-out"></span>
</button>`;
};

function Header({topic, setTopic, url, setUrl, connected}) {
  const forbiddenChars = ['$', '*', '+', '#', '/'];
  const onClick = () => {
    const isValidTopic = val => !forbiddenChars.some(char => val.includes(char));
    if (isValidTopic(topic)) {
      localStorage.setItem('topic', topic)
      setTopic(topic);
      window.location.reload();
    } else {
      setSaveResult('Error: The topic cannot contain these characters: ' + forbiddenChars);
    }
  };

  return html`
    <div class="top-0 py-2 border-b-2 bg-stone-100">
      <div class="flex px-4">
        <div class="flex grow gap-4">
          <h1 class="text-2xl font-semibold">Device Management Dashboard</h1>
        <//>
        <div class="flex gap-2 items-center">
          <div class="flex w-96 align-center justify-between gap-2">
            <span class=${LabelClass}>MQTT Server<//>
            <input disabled=${!connected} value=${url}
              onchange=${ev => setUrl(ev.target.value)} class=${InputClass} />
          <//>
          <div class="flex w-64 align-center justify-between gap-2">
            <span class=${LabelClass}>Root Topic<//>
            <input disabled=${!connected} value=${topic}
              onchange=${ev => setTopic(ev.target.value)} class=${InputClass} />
          <//>
          <${Button} icon=${Icons.bolt} onclick=${onClick} title=${connected ? 'Disconnect' : 'Connect'} />
        <//>
      <//>
    <//>`;
};

function Sidebar({devices, onclick}) {
  const Td = props => html`
    <td class="whitespace-nowrap border-b border-slate-200 py-2 px-4 pr-3 text-sm text-slate-900">${
      props.text}</td>`;

  const Device = ({d}) => html`
    <div class="hover:bg-stone-100 cursor-pointer flex gap-3 px-4 py-2 justify-between"
        onclick=${ev => onclick(d.id)}>
      <span>${d.id}</span>
      <span class="${BadgeClass} ${d.online ? Colors.green : Colors.red}"> ${d.online ? 'online' : 'offline'} <//>
    <//>`;

  return html`
    <div class="overflow-auto divide-y">
      <div class="${TitleClass} flex items-center px-4 py-2 bg-stone-100">
        Device List
      <//>
      ${(devices ? devices : []).map(d => h(Device, {d}))}
    <//>`;
};

function FirmwareStatus({title, info, children}) {
  const state =
      ['UNAVAILABLE', 'FIRST_BOOT', 'NOT_COMMITTED',
       'COMMITTED'][(info.status || 0) % 4];
  const valid = info.status > 0;
  return html`
    <div class="bg-white divide-y border rounded">
      <div class="${TitleClass} bg-white flex items-center px-4 py-2">
        ${title}
      <//>
      <div class="px-4 py-2 relative">
      <div class="my-1">Status: ${state}<//>
      <div class="my-1">CRC32: ${valid ? info.crc32.toString(16) : 'n/a'}<//>
      <div class="my-1">Size: ${valid ? info.size : 'n/a'}<//>
      <div class="my-1">Flashed at: ${
      valid ? new Date(info.timestamp * 1000).toLocaleString() : 'n/a'}<//>
      ${children}
      <//>
    <//>`;
};

function FirmwareUpdatePanel({no, name, current, previous, updated_at, refresh}) {
  const [color, setColor] = useState(Colors.green);
  const [otaStatus, setOtaStatus] = useState('up-to-date');
  const Descr = ({label, value}) => html`
    <div class="flex items-center justify-between">
      <span class=${LabelClass}>${label}<//>
      <span class="text-sm">${value}<//>
    <//>`;
  const onprogress = (len, total) => {
    setColor(Colors.info);
    setOtaStatus(`uploading, ${parseInt(len * 100 /total)}% done...`);
  }
  const setparam = (key, val) => fetch('api/settings/set', {
    method: 'POST',
    body: JSON.stringify({key, val}),
  }).then(handleFetchError);
  const onupload = function(ok, fileName, fileLength) {
    if (ok) {
      return setparam(`ecu.${no}.new`, fileName).then(refresh);
    } else {
      setColor(Colors.red), setOtaStatus('upload failed');
    }
  };
  const onfileselect = function(fileName, fileLength) {
    const ok = fileName.startsWith(name);
    if (!ok) alert(`Firmware file name must be ${name}.VERSION.BIN_OR_HEX`);
    return ok;
  }
  const update = rollback => new Promise(function(resolve, reject) {
    const opts = {method: 'POST', body: JSON.stringify({no, rollback})};
    const fail = () => (reject(), setOtaStatus('update error'), setColor(Colors.red));
    const finish = () => fetch('api/ecu/update/end', opts)
      .then(r => r.ok ? resolve() : reject())
      .then(refresh);
    const write = () => fetch('api/ecu/update/write', opts).then(r => {
      if (!r.ok) fail();
      //if (r.ok) r.json().then(r => console.log('Goo', r));
      if (r.ok) r.json().then(r => r == 1 ? write() : finish());
    });
    fetch('api/ecu/update/begin', opts).then(r => r.ok ? write() : fail());
  });
  const onflash = ev => update(false);
  const onrollback = ev => update(true);

  useEffect(function() {
    //setOtaStatus(anew ? 'update pending' : 'up-to-date');
    //setColor(anew ? Colors.yellow : Colors.green);
  }, []);

  return html`
<div class="divide-y border rounded bg-white">
  <div class="px-4 py-2 flex justify-between items-center rounded">
    <span class="${TitleClass}">Firmware Update<//>
    <div class="flex gap-2"><span class=${LabelClass}>Status:</span><span class="${BadgeClass} ${color}">${otaStatus}<//><//>
  <//>
  <div class="p-4 flex flex-col gap-1">
    <${Descr} label="Current firmware:" value=${current || 'n/a'} />
    <${Descr} label="Updated at:" value=${updated_at || 'n/a'} />
    <div class="flex items-center justify-between whitespace-nowrap">
      <span class=${LabelClass}>Previous firmware:<//>
      <span class="flex gap-1 items-center">
        <span class="text-sm">${previous}<//>
        <${Button} title="rollback" icon=${Icons.bolt} onclick=${onrollback}
          extraClass="w-24" disabled=${!previous} />
      <//>
    <//>
    <div class="flex items-center justify-between">
      <span class=${LabelClass}>New firmware:<//>
      <span class="flex gap-1 align-center justify-between">
        <${UploadFileButton} title="upload" url="api/fw/upload"
          onfileselect=${onfileselect} 
          onprogress=${onprogress} oncomplete=${onupload} />
        <${Button} title="flash" icon=${Icons.bolt} onclick=${onflash}
          extraClass="w-24" disabled=${!previous} />
      <//>
    <//>
  <//>
<//>`;
};

function DeviceSettingsPanel({device, publishFn, connected}) {
  const [config, setConfig] = useState(device.config);
  const logOptions = [[0, 'Disable'], [1, 'Error'], [2, 'Info'], [3, 'Debug']];

  const onSave = ev => publishFn('config.set', config).then(r => {
    //setDeviceConfig(device.id, config);
  }).catch(e => alert('Failure!'));

  // To delete device, set an empty retained message
  const onforget = function(ev) {
    MqttClient.publish(device.topic, '', {retain: true});
    location.reload();
  };
  const onloglevelchange = ev => setConfig(x => Object.assign({}, x, {log_level: parseInt(ev.target.value)}));
  const onpinschange = ev => {
    const pin_map = ev.target.value.split(/\s*,\s*/).map(x => parseInt(x));
    setConfig(x => Object.assign({}, x, {
      pin_map,                   // Send an updated pin map
      pin_count: pin_map.length, // And pin count
      pin_state:[]    // Remove pin_state from the request, as pin_map change can invalidate it
    }));
  }
  //console.log(config);

  return html`
<div class="divide-y border rounded bg-white">
  <div class="px-4 py-2 flex justify-between items-center rounded ${TitleClass}">
    <div class="flex gap-2 items-center"> Settings <//>
  <//>
  <div class="p-4 gap-3">
    <div class="flex justify-between my-1">
      <span class=${LabelClass}>Status<//>
      <span class="${BadgeClass} ${connected ? Colors.green : Colors.red}"> ${connected ? 'online' : 'offline'} <//>
    <//>
    <div class="flex justify-between my-1">
      <span class=${LabelClass}>Device ID<//>
      <span class="${LabelClass}">${device.id}<//>
    <//>
    <div class="grid grid-cols-2 my-1">
      <span class="${LabelClass}">Log Level<//>
      <select onchange=${onloglevelchange} class="w-full rounded font-normal border py-0.5 px-1 text-gray-600 focus:outline-none text-sm disabled:cursor-not-allowed" disabled=${!connected}>
        ${logOptions.map(v => html`<option value=${v[0]} selected=${v[0] == config.log_level}>${v[1]}<//>`) }
      <//>
    <//>
    <div class="grid grid-cols-2 my-1">
      <span class="${LabelClass}">Show Pins<//>
      <div class="flex w-full items-center">
        <input disabled=${!connected} value=${config.pin_map}
          onchange=${onpinschange} class=${InputClass}
          placeholder="comma-separated pin numbers" />
      <//>
    <//>

    <div class="mt-3 flex justify-between gap-2">
      <${Button} title="Forget this device"
        disabled=${device.online || !connected} icon=${Icons.delete}
        onclick=${onforget}/>
      <${Button} icon=${Icons.save} onclick=${onSave} title="Save Settings" disabled=${!device.online} />
    <//>
  <//>
<//> `;
};

function DeviceControlPanel({device, config, setConfig, publishFn, connected}) {
  const onclick = function(i) { // Send request to toggle pin i
    let configCopy = Object.assign({}, config);
    configCopy.pin_state[i] = !!configCopy.pin_state[i] ? 0 : 1;
    return publishFn('config.set', configCopy).catch(e => alert('Failure!'));
  };

  const Pin = i =>  html`
  <div class="inline-block my-1 bg-gray-100 rounded px-1 py-1">
    <div class="flex align-center justify-between">
      <span class="${LabelClass}">Pin ${config.pin_map[i]}<//>
      <${Toggle} onclick=${ev => onclick(i)}
        disabled=${!device.online || !connected}
        value=${config.pin_state[i]} />
    <//>
  <//>`;

  return html`
<div class="divide-y border rounded bg-white xbg-slate-100 my-y">
  <div class="px-4 py-2 flex justify-between items-center rounded ${TitleClass}">
    <div class="flex gap-2 items-center">Pin Control Panel<//>
  <//>
  <div class="p-4 grid grid-cols-2 lg:grid-cols-3 gap-2">
    ${(config.pin_map || []).map((_, i) => Pin(i))}
   <div><//>
  <//>
<//> `;
};

function DeviceDashboard({device, setDeviceConfig, publishFn, connected}) {
  const cfg = device && device.config ? device.config : DefaultDeviceConfig;
  const [localConfig, setLocalConfig] = useState(cfg);
  useEffect(() => setLocalConfig(cfg), [device]);
  if (!device || !localConfig) {
    return html`
    <div class="flex grow text-gray-400 justify-center items-center text-xl">
      No device selected. Click on a device on a sidebar
    <//>`;
  }

  return html`
  <div class="p-3 grid grid-cols-3 gap-2">
    <${DeviceSettingsPanel} device=${device} config=${localConfig} setConfig=${setLocalConfig} publishFn=${publishFn} connected=${connected} />
    <${DeviceControlPanel} device=${device} config=${localConfig} setConfig=${setLocalConfig} publishFn=${publishFn} connected=${connected} />
    <${FirmwareUpdatePanel} deviceID=${device.id} publishFn=${publishFn}
      disabled=${!device.online}
      info=${[localConfig.crnt_fw, localConfig.prev_fw]} />
  </div>`;
}

const App = function() {
  const [devices, setDevices] = useState([]);
  const [currentDevID, setCurrentDevID] = useState(localStorage.getItem('currentDevID') || '');
  const [loading, setLoading] = useState(true);
  const [url, setUrl] = useState(localStorage.getItem('url') || DefaultUrl);
  const [topic, setTopic] = useState(localStorage.getItem('topic') || DefaultTopic);
  const [error, setError] = useState(null);
  const [connected, setConnected] = useState(false);
  const responseHandlers = useRef({});

  function addResponseHandler(correlationId, handler) {
    responseHandlers[correlationId] = handler;
  }

  function removeResponseHandler(correlationId) {
    delete responseHandlers[correlationId];
  }

  const onRefresh = ev => window.location.reload();

  const initConn = () => {
    MqttClient = mqtt.connect(url, {connectTimeout: 5000, reconnectPeriod: 0});

    MqttClient.on('connect', () => {
      //console.log('Connected to the broker');
      setLoading(false);
      setError(null);  // Reset error state upon successful connection
      setConnected(true);

      const statusTopic = topic + '/+/status'
      const txTopic = topic + '/+/tx'

      const subscribe = (topic) => {
        MqttClient.subscribe(topic, (err) => {
          if (err) {
            console.error('Error subscribing to topic:', err);
            setError('Error subscribing to topic');
          } else {
            //console.log('Successfully subscribed to ', topic);
          }
        });
      };

      subscribe(statusTopic);
      subscribe(txTopic);
    });

    MqttClient.on('message', (topic, message) => {
      // console.log(`Received message from ${topic}: ${message.toString()}`);
      if (message.length == 0) return;
      let response;
      try {
        response = JSON.parse(message.toString());
      } catch (err) {
        console.error(err);
        return;
      }

      if (topic.endsWith('/status')) {
        let device = {topic: topic, id: topic.split('/')[1], config: DefaultDeviceConfig};
        const params = response.params;
        if (!params) {
          console.error('Invalid response');
          return;
        }
        device.online = params.status === 'online';
        setConnected(device.online);
        if (device.online) {
          device.config = params;
          if (!device.config.pins) device.config.pins = [];
        }
        setDevice(device)
      } else if (topic.endsWith('/tx')) {
        if (!response.id) {
          console.error('Invalid response');
          return;
        }
        const handler = responseHandlers[response.id];
        if (handler) {
          handler(response);
          removeResponseHandler(response.id);
        }
      }
    });

    MqttClient.on('error', (err) => {
      console.error('Connection error:', err);
      setError('Connection cannot be established.');
    });

    MqttClient.on('close', () => {
      if (!connected) {
        console.error('Failed to connect to the broker.');
        setError('Connection cannot be established.');
        setLoading(false);
      }
    });
  };

  useEffect(() => initConn(), []);

  const handlePublish = (methodName, parameters, timeout = 5000) => {
    return new Promise((resolve, reject) => {
      const randomIdGenerator = function(length) {
        return Math.random().toString(36).substring(2, length + 2);
      };
      const randomID = randomIdGenerator(40);
      const timeoutID = setTimeout(() => {
        removeResponseHandler(randomID);
        reject(new Error('Request timed out'));
      }, timeout);

      addResponseHandler(randomID, (messageData) => {
        clearTimeout(timeoutID);
        resolve(messageData);
      });

      if (currentDevID) {
        const rxTopic = topic + `/${currentDevID}/rx`;
        const rpcPayload = {method: methodName, id: randomID};
        if (parameters) {
          rpcPayload.params = parameters;
        }
        MqttClient.publish(rxTopic, JSON.stringify(rpcPayload));
      }
    });
  };

  const getDeviceByID = (deviceID) => devices.find(d => d.id === deviceID);

  const setDevice = (devData) => {
    setDevices(prevDevices => {
      const devIndex =
          prevDevices.findIndex(device => device.id === devData.id);
      if (devIndex !== -1) {
        if (!devData.online && !devData.config) {
          const updatedDevices = [...prevDevices];
          updatedDevices[devIndex].online = false;
          return updatedDevices;
        } else {
          return prevDevices.map(device => device.id === devData.id ? devData : device);
        }
      } else {
        return [...prevDevices, devData];
      }
    });
  };

  const setDeviceConfig = (deviceID, config) => {
    setDevices(prevDevices => {
      return prevDevices.map(device => {
        if (device.id === deviceID) {
          return {...device, config: config};
        }
        return device;
      });
    });
  };

  const onDeviceClick = (deviceID) => {
    const device = getDeviceByID(deviceID);
    if (device) {
      setCurrentDevID(device.id);
      localStorage.setItem('currentDevID', device.id);
    }
  };

  if (error) {
    return html`
    <div class="min-h-screen bg-slate-100 flex flex-col">
      <${Header}/>
      <div class="flex-grow flex items-center justify-center">
        <div class="bg-slate-300 p-8 rounded-lg shadow-md w-full max-w-md">
          <h1 class="text-2xl font-bold text-gray-700 mb-4 text-center">Connection Error</h1>
          <p class="text-gray-600 text-center mb-4">
            Unable to connect to the MQTT broker.
          </p>
          <div class="text-center relative">
            <${Button} title="Retry" onclick=${onRefresh}
              icon=${Icons.refresh} class="absolute top-4 right-4" />
          </div>
        </div>
      </div>
    </div>`;
  }

  return html`
  <div class="h-full flex flex-col">
    <${Header}
      topic=${topic} setTopic=${setTopic} url=${url}
      setUrl=${setUrl} connected=${connected}
    />
    <div class="flex grow overflow-auto">
      <div class="w-60 overflow-auto">
        <${Sidebar} devices=${devices} onclick=${onDeviceClick} />
      <//>
      <div class="grow bg-gray-200">
        <${DeviceDashboard}
          device=${getDeviceByID(currentDevID)} connected=${connected}
          setDeviceConfig=${setDeviceConfig} publishFn=${handlePublish} />
      <//>
    <//>
  <//>`;
};

window.onload = () => render(h(App), document.body);
