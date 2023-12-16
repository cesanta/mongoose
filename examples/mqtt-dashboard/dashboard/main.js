'use strict';
import {h, html, render, useEffect, useRef, useState} from './bundle.js';
import {Button, Colored, Icons, Notification, Setting, tipColors} from './components.js';

const Logo = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" viewBox="0 0 12.87 12.85"><defs><style>.ll-cls-1{fill:none;stroke:#000;stroke-miterlimit:10;stroke-width:0.5px;}</style></defs><g id="Layer_2" data-name="Layer 2"><g id="Layer_1-2" data-name="Layer 1"><path class="ll-cls-1" d="M12.62,1.82V8.91A1.58,1.58,0,0,1,11,10.48H4a1.44,1.44,0,0,1-1-.37A.69.69,0,0,1,2.84,10l-.1-.12a.81.81,0,0,1-.15-.48V5.57a.87.87,0,0,1,.86-.86H4.73V7.28a.86.86,0,0,0,.86.85H9.42a.85.85,0,0,0,.85-.85V3.45A.86.86,0,0,0,10.13,3,.76.76,0,0,0,10,2.84a.29.29,0,0,0-.12-.1,1.49,1.49,0,0,0-1-.37H2.39V1.82A1.57,1.57,0,0,1,4,.25H11A1.57,1.57,0,0,1,12.62,1.82Z"/><path class="ll-cls-1" d="M10.48,10.48V11A1.58,1.58,0,0,1,8.9,12.6H1.82A1.57,1.57,0,0,1,.25,11V3.94A1.57,1.57,0,0,1,1.82,2.37H8.9a1.49,1.49,0,0,1,1,.37l.12.1a.76.76,0,0,1,.11.14.86.86,0,0,1,.14.47V7.28a.85.85,0,0,1-.85.85H8.13V5.57a.86.86,0,0,0-.85-.86H3.45a.87.87,0,0,0-.86.86V9.4a.81.81,0,0,0,.15.48l.1.12a.69.69,0,0,0,.13.11,1.44,1.44,0,0,0,1,.37Z"/></g></g></svg>`;
const DefaultTopic = 'mg_mqtt_dashboard';
const DefaultUrl = location.protocol == 'https:'
  ? 'wss://broker.hivemq.com:8884/mqtt'
  : 'ws://broker.hivemq.com:8000/mqtt';
const DefaultDeviceConfig = {pins: [], log_level: 0};
let MqttClient;

function Header({topic, setTopic, url, setUrl, connected}) {
  const forbiddenChars = ['$', '*', '+', '#', '/'];
  const onClick = () => {
    const isValidTopic = val => !forbiddenChars.some(char => val.includes(char));
    if (isValidTopic(topic)) {
      localStorage.setItem('topic', topic)
      setTopicFn(topic);
      window.location.reload();
    } else {
      setSaveResult('Error: The topic cannot contain these characters: ' + forbiddenChars);
    }
  };
  return html`
    <div class="top-0 py-2 border-b-2 bg-stone-100">
      <div class="flex px-4">
        <div class="flex grow gap-4">
          <h1 class="text-2xl font-semibold">MQTT Dashboard</h1>
          <small class="text-gray-500 mt-2.5">powered by Mongoose<//>
        <//>
        <div class="flex gap-2 items-center">
          <${Setting} value=${url} addonLeft="MQTT Server" cls="py-1 w-96" disabled=${connected} />
          <${Setting} value=${topic} addonLeft="Root Topic" cls="py-1" disabled=${connected} />
          <${Button} icon=${Icons.link} onclick=${onClick} title=${connected ? 'Disconnect' : 'Connect'} />
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
      <${Colored}
        colors=${d.online ? tipColors.green : tipColors.red}
        text=${d.online ? 'online' : 'offline'} />
    <//>`;

  return html`
    <div class="overflow-auto divide-y border-r xbasis-60 w-60 flex-none">
      <div class="font-light uppercase flex items-center text-gray-600 px-4 py-2 bg-stone-100">
        Devices
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
      <div class="font-light bg-white uppercase flex items-center text-gray-600 px-4 py-2">
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

function UploadFileButton(props) {
  const [upload, setUpload] = useState(null);  // Upload promise
  const [status, setStatus] = useState({});    // Current upload status
  const btn = useRef(null);
  const input = useRef(null);

  const setStatusByID = function(message, id) {
    setStatus(prvStatus => ({...prvStatus, [id]: message}))
  };

  // Send a large file chunk by chunk
  const sendFileData = function(fileName, fileData, chunkSize) {
    return new Promise(function(resolve, reject) {
      const finish = ok => {
        setUpload(null);
        const res = props.onupload ?
            props.onupload(ok, fileName, fileData.length) :
            null;
        if (res && typeof (res.catch) === 'function') {
          res.catch(() => false).then(() => ok ? resolve() : reject());
        } else {
          ok ? resolve() : reject();
        }
      };

      const sendChunk = function(offset) {
        var chunk = fileData.subarray(offset, offset + chunkSize) || '';
        var ok;
        setStatusByID(
            'Uploading ' + fileName + ', bytes ' + offset + '..' +
                (offset + chunk.length) + ' of ' + fileData.length,
            props.id);
        const params = {
          chunk: btoa(String.fromCharCode.apply(null, chunk)),
          offset: offset,
          total: fileData.length
        };
        props.publishFn('ota.upload', params)
            .then(function(res) {
              if (res.result === 'ok' && chunk.length > 0)
                sendChunk(offset + chunk.length);
              ok = res.result === 'ok';
              return res;
            })
            .then(function(res) {
              if (!ok)
                setStatusByID('Error: ' + res.error, props.id),
                    finish(ok);              // Fail
              if (chunk.length > 0) return;  // More chunks to send
              setStatus(x => x + '. Done, resetting device...');
              finish(ok);  // All chunks sent
            })
            .catch(e => {
              setStatusByID('Error: timed out', props.id);
              finish(false)
            });
      };
      // setFailed(false);
      sendChunk(0);
    });
  };

  const onchange = function(ev) {
    if (!ev.target.files[0]) return;
    let r = new FileReader(), f = ev.target.files[0];
    r.readAsArrayBuffer(f);
    r.onload = function() {
      setUpload(sendFileData(f.name, new Uint8Array(r.result), 2048));
      ev.target.value = '';
      ev.preventDefault();
      btn && btn.current.base.click();
    };
  };

  const onclick = function(ev) {
    let fn;
    setUpload(x => fn = x);
    if (!fn) input.current.click();  // No upload in progress, show file dialog
    return fn;
  };

  if (props.clean) {
    setStatusByID(null, props.id)
    props.setCleanFn(false)
  }

  return html`
<div class="inline-flex flex-col ${props.class}">
  <input class="hidden" type="file" ref=${input} onchange=${onchange} accept=${
      props.accept} />
  <${Button} title=${props.title} icon=${Icons.download} onclick=${
      onclick} ref=${btn} colors=${props.colors} disabled=${props.disabled} />
  <div class="py-2 text-sm text-slate-400 ${status || 'hidden'}">${
      status[props.id]}<//>
<//>`;
};

function FirmwareUpdate({publishFn, disabled, info, deviceID}) {
  const [clean, setClean] = useState(false)
  const refresh = () => {};
  useEffect(refresh, []);
  const oncommit = ev => {
    publishFn('ota.commit')
  };
  const onreboot = ev => {
    publishFn('device.reset')
  };
  const onrollback = ev => {
    publishFn('ota.rollback')
  };
  const onerase = ev => {};
  const onupload = function(ok, name, size) {
    if (!ok) return false;
    return new Promise(r => setTimeout(ev => {
                         refresh();
                         r();
                       }, 3000)).then(r => setClean(true));
  };

  const defaultInfo = {status: 0, crc32: 0, size: 0, timestamp: 0};
  return html`
  <div class="bg-slate-200 border rounded text-ellipsis overflow-auto mt-4">
    <div class="bg-white px-4 py-2 flex items-center justify-between font-light uppercase text-gray-600">
      Over-the-air firmware updates
    </div>
    <div class="gap-1 grid grid-cols-3 lg:grid-cols-3">
      <${FirmwareStatus} title="Current firmware image"
        info=${info[0] ? info[0] : defaultInfo}>
        <div class="flex flex-wrap gap-2">
          <${Button} title="Commit this firmware" onclick=${oncommit}
            icon=${Icons.thumbUp} disabled=${disabled} cls="w-full" />
        <//>
      <//>
      <${FirmwareStatus} title="Previous firmware image" info=${
      info[1] ? info[1] : defaultInfo}>
        <${Button} title="Rollback to this firmware" onclick=${onrollback}
          icon=${Icons.backward} disabled=${disabled} cls="w-full" />
      <//>
      <div class="bg-white xm-4 divide-y border rounded flex flex-col">
        <div class="font-light uppercase flex items-center text-gray-600 px-4 py-2">
          Device control
        <//>
        <div class="px-4 py-3 flex flex-col gap-2 grow">
          <${UploadFileButton}
            title="Upload new firmware: choose .bin file:"
              publishFn=${publishFn} onupload=${onupload} clean=${clean}
              setCleanFn=${setClean} disabled=${disabled}
              id=${deviceID} url="api/firmware/upload" accept=".bin,.uf2" />
          <div class="grow"><//>
          <${Button} title="Reboot device" onclick=${onreboot}
            icon=${Icons.refresh} disabled=${disabled} cls="w-full" />
          <${Button} title="Erase last sector" onclick=${onerase}
            icon=${Icons.doc} disabled=${disabled} cls="w-full hidden" />
        <//>
      <//>
    <//>
  <//>`;
};

function DeviceControlPanel({device, setDeviceConfig, publishFn, connected}) {
  const cfg = device && device.config ? device.config : DefaultDeviceConfig;
  const [localConfig, setLocalConfig] = useState(cfg);
  const [saveResult, setSaveResult] = useState(null);

  useEffect(() => setLocalConfig(cfg), [device]);

  const logOptions = [[0, 'Disable'], [1, 'Error'], [2, 'Info'], [3, 'Debug']];
  const mksetfn = k => (v => setLocalConfig(x => Object.assign({}, x, {[k]: v})));

  const onSave = ev => publishFn('config.set', localConfig).then(r => {
    setDeviceConfig(device.id, localConfig)
    setSaveResult('Success!')
  }).catch(e => {
    setDeviceConfig(device.id, device.config)
    setSaveResult('Failed!')
  })

  if (!device || !localConfig) {
    return html`
    <div class="flex grow text-gray-400 justify-center items-center text-xl">
      No device selected. Click on a device on a sidebar
    <//>`;
  }

  // To delete device, set an empty retained message
  const onforget = function(ev) {
    MqttClient.publish(device.topic, '', {retain: true});
    location.reload();
  };

  const mksetpin = (pin, val) => setLocalConfig(x => {
    let pins = x.pins.slice();
    pins[pin] = val ? 0 : 1;
    return Object.assign({}, x, {pins});
  });

  const settingstyle = 'inline-block mr-8 my-1 bg-gray-100 rounded px-2';
  const Pin = (value, pin) =>  html`
  <div class="${settingstyle} w-32">
    <${Setting}
      setfn=${ev => mksetpin(pin, value)}
      type="switch" value=${value} xcls="py-1 w-32 overflow-auto"
      disabled=${!device.online || !connected} title="Pin ${pin}" />
  <//>`;

  return html`
  <div class="flex-1 xgrow overflow-auto bg-gray-200 xpx-4 xpy-2 p-3">
    <div class="divide-y border rounded bg-white xbg-slate-100 my-y">

      <div class="px-4 py-2 flex justify-between items-center font-light uppercase text-gray-600 rounded">
        <div class="flex gap-2 items-center">
          <span>Device ${device.id}<//>
          ${html`<${Colored}
            colors=${device.online ? tipColors.green : tipColors.red}
            text=${ device.online ? 'online' : 'offline'} />`}
        <//>
        <${Button} title="Forget this device"
          disabled=${device.online || !connected}
          icon=${Icons.fail} onclick=${onforget}/>
      <//>

      <div class="p-4 xgrid gap-3">
        ${localConfig.pins.map((val, pin) => Pin(val, pin))}
        <div><//>

        <div class="${settingstyle} w-48">
          <${Setting} title="Log Level" type="select"
            value=${localConfig.log_level} setfn=${mksetfn('log_level')}
            options=${logOptions} disabled=${!device.online}/>
        <//>

        <div class="mt-3 flex justify-end">
          ${saveResult && html`<${Notification} ok=${saveResult === 'Success!'}
            text=${saveResult} close=${() => setSaveResult(null)} />`}
          <${Button} icon=${Icons.save} onclick=${onSave}
            title="Save Settings" disabled=${!device.online} />
        <//>
      <//>
    <//>

    <${FirmwareUpdate} deviceID=${device.id} publishFn=${publishFn}
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

      subscribe(statusTopic)
      subscribe(txTopic)
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
        device.online = params.status === 'online'
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
      <${Sidebar} devices=${devices} onclick=${onDeviceClick} />
      <${DeviceControlPanel}
        device=${getDeviceByID(currentDevID)} connected=${connected}
        setDeviceConfig=${setDeviceConfig} publishFn=${handlePublish}
      />
    <//>
  <//>`;
};

window.onload = () => render(h(App), document.body);
