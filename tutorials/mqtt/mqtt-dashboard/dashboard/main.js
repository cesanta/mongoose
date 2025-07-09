'use strict';
import {h, html, render, useEffect, useRef, useSignal} from './bundle.js';

const DefaultTopic = 'mg_mqtt_dashboard';
const DefaultUrl = location.protocol == 'https:'
  ? 'wss://broker.hivemq.com:8884/mqtt'
  : 'ws://broker.hivemq.com:8000/mqtt';
// const Delay = (ms, val) => new Promise(resolve => setTimeout(resolve, ms, val));
// const handleFetchError = r => r.ok || alert(`Error: ${r.statusText}`);
const LabelClass = 'text-sm truncate font-medium my-auto whitespace-nowrap';
const BadgeClass = 'flex-inline text-sm rounded-md rounded px-2 py-0.5 ring-1 ring-inset';
const InputClass = 'font-normal text-sm rounded w-full flex-1 py-0.5 px-2 text-gray-700 placeholder:text-gray-400 focus:outline-none disabled:cursor-not-allowed disabled:bg-gray-100 disabled:text-gray-500 rounded border';
const TitleClass = 'font-semibold';
const Colors = {
  green: 'bg-green-100 text-green-900 ring-green-300',
  yellow: 'bg-yellow-100 text-yellow-900 ring-yellow-300',
  info: 'bg-zinc-100 text-zinc-900 ring-zinc-300',
  red: 'bg-red-100 text-red-900 ring-red-300',
};

let MqttClient;

const Help = () => html`
<div class="p-2 w-96 text-slate-600 bg-amber-100 overflow-auto text-sm">
  <div class="py-2">This is a simple demonstration of the functional device dashboard
    that manages a fleet of devices via an MQTT server. Source code
    is <a class="text-blue-600" href="https://github.com/cesanta/mongoose/tree/master/tutorials/mqtt/mqtt-dashboard">available on GitHub<//>.
  <//>
  <div class="py-2">
  For the sake of simplicity, this dashboard does not implement authentication.
  No external storage is used either to keep device list: for that, retained
  MQTT messages are utilised. When a device goes online, it publishes its
  state to the {root_topic}/{device_id}/status topic
  - LED status and firmware version.
  <//>
  <div class="py-2">
  The last will message triggers
  the "offline" message to the same topic. This is how this web page
  is able to track online/offline status of devices.
  <img src="mqtt.svg" alt="diagram" />
  <//>
  <div class="py-1">
  See developer console for the list of MQTT messages exchanged with
  the MQTT server.
  <//>
<//>`;

export function Button({title, onclick, disabled, extraClass, ref, colors}) {
  const sigSpin = useSignal(false);
  const cb = function(ev) {
    const res = onclick ? onclick() : null;
    if (res && typeof (res.catch) === 'function') {
      sigSpin.value = true;
      res.catch(() => false).then(() => sigSpin.value = false);
    }
  };
  if (!colors) colors = 'bg-blue-600 hover:bg-blue-500 disabled:bg-blue-400';
  return html`
<button type="button" class="inline-flex justify-center items-center gap-2 rounded px-2.5 py-0.5 text-sm font-semibold text-white shadow-sm ${colors} ${extraClass} ${sigSpin.value ? 'animate-pulse' : ''}"
  ref=${ref} onclick=${cb} disabled=${disabled || sigSpin.value} >
  ${title}
<//>`
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

function Header({sigTopic, sigUrl, connected}) {
  const forbiddenChars = ['$', '*', '+', '#', '/'];
  const onClick = () => {
    const isValidTopic = val => !forbiddenChars.some(char => val.includes(char));
    if (isValidTopic(topic)) {
      localStorage.setItem('topic', topic)
      sigTopic.value = topic;
      window.location.reload();
    } else {
      setSaveResult('Error: The topic cannot contain these characters: ' + forbiddenChars);
    }
  };

  return html`
    <div class="py-2 bg-slate-700 text-slate-50">
      <div class="flex px-4">
        <div class="flex grow gap-4">
          <h1 class="text-2xl font-semibold">IoT Fleet Management Dashboard</h1>
        <//>
        <div class="flex gap-2 items-center">
          <div class="flex w-96 align-center justify-between gap-2">
            <span class=${LabelClass}>MQTT Server<//>
            <input disabled=${!connected} value=${sigUrl.value}
              onchange=${ev => sigUrl.value = ev.target.value} class=${InputClass} />
          <//>
          <div class="flex w-64 align-center justify-between gap-2">
            <span class=${LabelClass}>Root Topic<//>
            <input disabled=${!connected} value=${sigTopic.value}
              onchange=${ev => sigTopic.value = ev.target.value} class=${InputClass} />
          <//>
          <${Button} extraClass="w-32" onclick=${onClick} title=${connected ? 'Disconnect' : 'Connect'} />
        <//>
      <//>
    <//>`;
};

function Sidebar({devices, onclick}) {
  const Td = props => html`
    <td class="whitespace-nowrap border-b py-2 px-4 pr-3 text-sm text-slate-700">${props.text}</td>`;

  const Device = ({d}) => html`
    <div class="hover:bg-stone-100 cursor-pointer flex gap-3 px-4 py-2 justify-between"
        onclick=${ev => onclick(d.id)}>
      <span>${d.id}</span>
      <span class="${BadgeClass} ${d.online ? Colors.green : Colors.red} w-16 text-center"> ${d.online ? 'online' : 'offline'} <//>
    <//>`;

  return html`
    <div class="overflow-auto divide-y">
      <div class="${TitleClass} flex items-center px-4 py-2 text-slate-400">
        Device List
      <//>
      ${(devices ? devices : []).map(d => h(Device, {d}))}
    <//>`;
};

export function File({accept, fn, ...rest}) {
  const btn = useRef(null);
  const input = useRef(null);

  const oncancel = function(ev) { input.resolve(); input.resolve = null; };
  const onchange = function(ev) {
    if (!ev.target.files[0]) { input.resolve(); input.resolve = null; return; }
    const f = ev.target.files[0];
    const reader = new FileReader();
    reader.readAsArrayBuffer(f);
    reader.onload = function() {
      fn && fn(reader.result, f.type).then(() => {
        input.resolve();
      }).finally(() => {
        input.resolve = null;
      });
    };
    btn.current && btn.current.base.click();
    ev.target.value = '';
    ev.preventDefault();
  };
  const onclick = function() {
    if (!input.fn) input.current.click();
    return new Promise(resolve => { input.resolve = resolve; });
  };

  return html`
<span>
  <input style="display:none;" type="file" ref=${input} oncancel=${oncancel} onchange=${onchange} accept=${accept} />
  <${Button} onclick=${onclick} ref=${btn} ...${rest} />
<//>`;
};

function arrayBufferToBase64Async(buffer) {
  return new Promise((resolve, reject) => {
    const blob = new Blob([buffer]);
    const reader = new FileReader();
    reader.onload = () => {
      const dataUrl = reader.result; // reader.result is like "data:application/octet-stream;base64,AAAA..."
      resolve(dataUrl.split(",", 2)[1]);
    };
    reader.onerror = reject;
    reader.readAsDataURL(blob);
  });
}

function FirmwareUpdatePanel({device, rpc, connected}) {

  const fn = function (fileData) {
    // Split file in chunks
    let chunkSize = 4096, offset = 0;
    const chunks = Array.from({ length: Math.ceil(fileData.byteLength / chunkSize) }, (_, i) =>
      fileData.slice(i * chunkSize, i * chunkSize + chunkSize)
    );
    function next(resolve, reject) {
      const chunk = chunks.shift();
      if (chunks.length == 0)  resolve();
      return arrayBufferToBase64Async(chunk)
        .then(encoded => rpc('ota.upload', {
          offset: offset,
          total: fileData.byteLength,
          chunk: encoded,
        }))
        .then(response => {
          if (response.result == 'ok') {
            next(resolve, reject);
            offset += chunkSize;
          } else {
            reject();
          }
        });
    };
    return new Promise((resolve, reject) => next(resolve, reject));
  };

  return html`
<div class="divide-y border rounded bg-white w-72">
  <div class="px-4 py-2 flex justify-between items-center rounded">
    <span class="${TitleClass}">Firmware Update<//>
  <//>
  <div class="p-4 flex flex-col gap-1">
    <div class="flex justify-between align-center gap-2">
      <div class="${LabelClass}">Current firmware version<//>
      <div class="text-bold">${device.firmware_version || '??'}<//>
    <//>

    <div class="flex justify-between align-center gap-2">
      <div class="${LabelClass}">Upload new firmware<//>
      <${File} title="..." disabled=${!device.online || !connected} fn=${fn} />
    <//>
  <//>
<//>`;
};

function LedControlPanel({device, rpc, connected}) {
  const ontoggle = () => rpc('state.set', {led_status: !device.led_status});

  return html`
<div class="divide-y border rounded bg-white xbg-slate-100 my-y w-64">
  <div class="px-4 py-2 flex justify-between items-center rounded ${TitleClass}">
    <div class="flex gap-2 items-center">LED Control Panel<//>
  <//>
  <div class="p-4 flex justify-between align-center gap-2">
    <div class="${LabelClass}">Toggle LED<//>
    <${Toggle} onclick=${ontoggle} disabled=${!device.online || !connected} value=${device.led_status} />
  <//>
<//> `;
};

function DeviceDashboard({device, rpc, connected}) {
  // To delete device, set an empty retained message
  const onforget = function(ev) {
    MqttClient.publish(device.topic, '', {retain: true});
    location.reload();
  };

  if (!device) {
    return html`
    <div class="flex grow text-gray-400 justify-center items-center text-xl h-full">
      No device selected. Click on a device on a sidebar
    <//>`;
  }

  return html`
  <div class="p-3 flex flex-col gap-2">
    <div class="text-xl flex items-center gap-3">
      <span class="text-slate-400">Device ${device.id}<//>
      <${Button} title="Forget this device" onclick=${onforget}/>
    <//>
    <div class="flex gap-2">
      <${LedControlPanel} device=${device} rpc=${rpc} connected=${connected} />
      <${FirmwareUpdatePanel} device=${device} rpc=${rpc} connected=${connected} />
    <//>
  <//>`;
}

const App = function() {
  const sigDevices = useSignal([]);
  const sigCurrentDevID = useSignal(localStorage.getItem('currentDevID') || '');
  const sigUrl = useSignal(localStorage.getItem('url') || DefaultUrl);
  const sigTopic = useSignal(localStorage.getItem('topic') || DefaultTopic);
  const sigError = useSignal(null);
  const sigLoading = useSignal(true);
  const sigConnected = useSignal(false);
  const responseHandlers = useRef({});

  const getDeviceByID = (deviceID) => sigDevices.value.find(d => d.id === deviceID);


  function addResponseHandler(correlationId, handler) {
    responseHandlers[correlationId] = handler;
  }

  function removeResponseHandler(correlationId) {
    delete responseHandlers[correlationId];
  }

  const onRefresh = () => window.location.reload();

  const initConn = () => {
    MqttClient = mqtt.connect(sigUrl.value, {connectTimeout: 5000, reconnectPeriod: 0});

    MqttClient.on('connect', () => {
      //console.log('Connected to the broker');
      sigLoading.value = false;
      sigError.value = null; // Reset error state upon successful connection
      sigConnected.value = true;

      const statusTopic = sigTopic.value + '/+/status'
      const txTopic = sigTopic.value + '/+/tx'

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
      console.log(`Received message from ${topic}: ${message.toString()}`);
      if (message.length == 0) return;
      let response;
      try {
        response = JSON.parse(message.toString());
      } catch (err) {
        console.error(err);
        return;
      }

      if (topic.endsWith('/status')) {
        if (!response.params) {
          console.error('Invalid response');
          return;
        }
        let device = Object.assign(response.params, {topic: topic, id: topic.split('/')[1]});
        device.online = response.params.status === 'online';
        const devices = sigDevices.value.filter(d => d.id !== device.id);
        devices.push(device);
        devices.sort((a, b) => a.online && !b.online ? -1
          :!a.online && b.online ? 1
          : a.id < b.id ? -1 : 1);
        sigDevices.value = devices;
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
      sigError.value = 'Connection cannot be established.';
    });

    MqttClient.on('close', () => {
      if (!sigConnected.value) {
        console.error('Failed to connect to the broker.');
        sigError.value = 'Connection cannot be established.';
        sigLoading.value = false;
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

      if (sigCurrentDevID.value) {
        const rxTopic = sigTopic.value + `/${sigCurrentDevID.value}/rx`;
        const rpcPayload = {method: methodName, id: randomID};
        if (parameters) {
          rpcPayload.params = parameters;
        }
        console.log(`Sending message to ${rxTopic}: ${JSON.stringify(rpcPayload)}`);
        MqttClient.publish(rxTopic, JSON.stringify(rpcPayload));
      }
    });
  };

  const onDeviceClick = (deviceID) => {
    const device = getDeviceByID(deviceID);
    if (device) {
      sigCurrentDevID.value = device.id;
      localStorage.setItem('currentDevID', device.id);
    }
  };

  if (sigError.value) {
    return html`
    <div class="min-h-screen bg-slate-100 flex flex-col">
      <${Header} sigTopic=${sigTopic} sigUrl=${sigUrl} />
      <div class="flex-grow flex items-center justify-center">
        <div class="bg-slate-300 p-8 rounded-lg shadow-md w-full max-w-md">
          <h1 class="text-2xl font-bold text-gray-700 mb-4 text-center">Connection Error</h1>
          <p class="text-gray-600 text-center mb-4">
            Unable to connect to the MQTT broker.
          </p>
          <div class="text-center relative">
            <${Button} title="Retry" onclick=${onRefresh} class="absolute top-4 right-4" />
          </div>
        </div>
      </div>
    </div>`;
  }

  return html`
  <div class="h-full flex flex-col">
    <${Header} sigTopic=${sigTopic} sigUrl=${sigUrl} connected=${sigConnected.value} />
    <div class="flex grow overflow-auto">
      <div class="w-64 overflow-auto">
        <${Sidebar} devices=${sigDevices.value} onclick=${onDeviceClick} />
      <//>
      <div class="grow bg-gray-200">
        <${DeviceDashboard}
          device=${getDeviceByID(sigCurrentDevID.value)} connected=${sigConnected.value}
          rpc=${handlePublish} />
      <//>
      <${Help} />
    <//>
  <//>`;
};

window.onload = () => render(h(App), document.body);
