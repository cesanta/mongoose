'use strict';
import { h, render, useState, useEffect, useRef, html } from  './bundle.js';
import { Icons, Setting, Button, tipColors, Colored, Notification} from './components.js';

const Logo = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" viewBox="0 0 12.87 12.85"><defs><style>.ll-cls-1{fill:none;stroke:#000;stroke-miterlimit:10;stroke-width:0.5px;}</style></defs><g id="Layer_2" data-name="Layer 2"><g id="Layer_1-2" data-name="Layer 1"><path class="ll-cls-1" d="M12.62,1.82V8.91A1.58,1.58,0,0,1,11,10.48H4a1.44,1.44,0,0,1-1-.37A.69.69,0,0,1,2.84,10l-.1-.12a.81.81,0,0,1-.15-.48V5.57a.87.87,0,0,1,.86-.86H4.73V7.28a.86.86,0,0,0,.86.85H9.42a.85.85,0,0,0,.85-.85V3.45A.86.86,0,0,0,10.13,3,.76.76,0,0,0,10,2.84a.29.29,0,0,0-.12-.1,1.49,1.49,0,0,0-1-.37H2.39V1.82A1.57,1.57,0,0,1,4,.25H11A1.57,1.57,0,0,1,12.62,1.82Z"/><path class="ll-cls-1" d="M10.48,10.48V11A1.58,1.58,0,0,1,8.9,12.6H1.82A1.57,1.57,0,0,1,.25,11V3.94A1.57,1.57,0,0,1,1.82,2.37H8.9a1.49,1.49,0,0,1,1,.37l.12.1a.76.76,0,0,1,.11.14.86.86,0,0,1,.14.47V7.28a.85.85,0,0,1-.85.85H8.13V5.57a.86.86,0,0,0-.85-.86H3.45a.87.87,0,0,0-.86.86V9.4a.81.81,0,0,0,.15.48l.1.12a.69.69,0,0,0,.13.11,1.44,1.44,0,0,0,1,.37Z"/></g></g></svg>`;

const url = 'ws://broker.hivemq.com:8000/mqtt'
const default_topic = 'topic_mg_device'
let client;

function Header( {topic, setTopicFn} ) {
  const [inputValue, setInputValue] = useState(topic);
  const [saveResult, setSaveResult] = useState(null);
  const onInput = (ev) => setInputValue(ev.target.value);
  const forbiddenChars = ['$', '*', '+', '#', '/'];

  const onClick = () => {
    const isValidTopic = (value) => {
      return !forbiddenChars.some(char => value.includes(char));
    };

    if (isValidTopic(inputValue)) {
      localStorage.setItem('topic', inputValue)
      setTopicFn(inputValue);
      window.location.reload();
    } else {
      setSaveResult("Error: The topic cannot contain these characters: " + forbiddenChars)
    }
  };

  return html`
    <div class="bg-white sticky top-0 z-[48] w-full border-b-2 border-gray-300 py-3 shadow-md transition-all duration-300 transform">
      <div class="mx-auto flex justify-between items-center px-5">
        <div class="text-gray-800 font-semibold tracking-wide">
          <h1 class="text-2xl">MQTT Dashboard</h1>
        </div>
        <div class="flex space-x-4">
          <div class="text-lg font-medium text-gray-700 text-center">
            <div class="text-gray-500">MQTT Server:</div>
            <code class="bg-gray-100 font-normal text-sm rounded w-full flex-1 py-0.5 px-2 text-gray-700" style="font-size: 0.8em; display: block; white-space: nowrap; overflow: hidden; text-overflow: ellipsis; padding: 1px; background-color: #f9f9f9; border: 1px solid #ddd; border-radius: 5px;">
              ${url}
            </code>
          </div>
          <div style="border-left: 1px solid #ddd; height: 50px;"></div>
          <div class="text-lg font-medium text-gray-700 text-center">
            <div class="text-gray-500">Topic:</div>
            <div class="flex w-full items-center rounded border shadow-sm bg-white">
              <input type="text" value=${inputValue} onchange=${onInput} step="1" class="bg-white font-normal text-sm rounded w-full flex-1 py-0.5 px-2 text-gray-700 placeholder:text-gray-400 focus:outline-none disabled:cursor-not-allowed disabled:bg-gray-100 disabled:text-gray-500"/>
            </div>
          </div>
          <div class="mb-1 mt-3 flex place-content-end"><${Button} icon=${Icons.save} onclick=${onClick} title="Change topic" /><//>
          ${saveResult && html`<${Notification} ok=${saveResult === "Success!"}
            text=${saveResult} close=${() => setSaveResult(null)} />`}
        </div>
      </div>
    </div>
  `;
};

function Devices({ devices, onClickFn }) {
  const Td = props => html`
    <td class="whitespace-nowrap border-b border-slate-200 py-2 px-4 pr-3 text-sm text-slate-900">${props.text}</td>`;

    const Device = ({ d }) => html`
    <tr class="hover:bg-slate-100 cursor-pointer" onClick=${() => onClickFn(d.id)}>
      <td class="border-b-2 border-slate-300 py-2 text-center align-middle">
        <div class="flex flex-col items-center justify-center h-full">
          <span>ID ${d.id}</span>
          <${Colored} colors=${d.online ? tipColors.green : tipColors.red} text=${d.online ? 'online' : 'offline'} />
        </div>
      </td>
    </tr>`;

  return html`
    <div class="m-4 divide-y divide-gray-200 overflow-auto rounded bg-white">
      <div class="font-semibold flex items-center text-gray-600 px-3 justify-center whitespace-nowrap">
        <div class="font-bold flex items-center text-gray-600">
          Devices list
        </div>
      <//>
      <div class="inline-block min-w-full align-middle" style="max-height: 82vh; overflow: auto;">
        <table class="w-full table table-bordered">
          <tbody>
            ${(devices ? devices : []).map(d => h(Device, { d }))}
          </tbody>
        </table>
      </div>
    </div>`;
};

function FirmwareStatus({title, info, children}) {
  const state = ['UNAVAILABLE', 'FIRST_BOOT', 'NOT_COMMITTED', 'COMMITTED'][(info.status || 0) % 4];
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
      <div class="my-1">Flashed at: ${valid ? new Date(info.timestamp * 1000).toLocaleString() : 'n/a'}<//>
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
        const res = props.onupload ? props.onupload(ok, fileName, fileData.length) : null;
        if (res && typeof (res.catch) === 'function') {
          res.catch(() => false).then(() => ok ? resolve() : reject());
        } else {
          ok ? resolve() : reject();
        }
      };

      const sendChunk = function(offset) {
        var chunk = fileData.subarray(offset, offset + chunkSize) || '';
        var ok;
        setStatusByID('Uploading ' + fileName + ', bytes ' + offset + '..' +
          (offset + chunk.length) + ' of ' + fileData.length, props.id);
        const params = {chunk: btoa(String.fromCharCode.apply(null, chunk)), offset: offset, total: fileData.length}
        props.publishFn("ota.upload", params)
          .then(function(res) {
            if (res.result === "ok" && chunk.length > 0) sendChunk(offset + chunk.length);
            ok = res.result === "ok";
            return res;
          })
          .then(function(res) {
            if (!ok) setStatusByID('Error: ' + res.error, props.id), finish(ok); // Fail
            if (chunk.length > 0) return; // More chunks to send
            setStatus(x => x + '. Done, resetting device...');
            finish(ok); // All chunks sent
          })
          .catch(e => {
            setStatusByID("Error: timed out", props.id);
            finish(false)
          });
      };
      //setFailed(false);
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
    let fn; setUpload(x => fn = x);
    if (!fn) input.current.click();  // No upload in progress, show file dialog
    return fn;
  };

  if (props.clean) {
    setStatusByID(null, props.id)
    props.setCleanFn(false)
  }

  return html`
<div class="inline-flex flex-col ${props.class}">
  <input class="hidden" type="file" ref=${input} onchange=${onchange} accept=${props.accept} />
  <${Button} title=${props.title} icon=${Icons.download} onclick=${onclick} ref=${btn} colors=${props.colors} disabled=${props.disabled} />
  <div class="py-2 text-sm text-slate-400 ${status || 'hidden'}">${status[props.id]}<//>
<//>`;
};

function FirmwareUpdate({ publishFn, disabled, info, deviceID }) {
  const [clean, setClean] = useState(false)
  const refresh = () => {};
  useEffect(refresh, []);
  const oncommit = ev => { publishFn("ota.commit") };
  const onreboot = ev => { publishFn("device.reset")};
  const onrollback = ev => { publishFn("ota.rollback") };
  const onerase = ev => {};
  const onupload = function(ok, name, size) {
    if (!ok) return false;
    return new Promise(r => setTimeout(ev => { refresh(); r(); }, 3000)).then(r => setClean(true));
  };

  const defaultInfo = {status: 0, crc32: 0, size: 0, timestamp: 0}
  return html`
  <div class="bg-slate-200 border rounded-md text-ellipsis overflow-auto">
    <div class="bg-slate-100 px-4 py-2 flex items-center justify-between font-light uppercase text-gray-600">
      Over-the-air firmware updates
    </div>
    <div class="gap-1 grid grid-cols-3 lg:grid-cols-3">
      <${FirmwareStatus} title="Current firmware image" info=${info[0] ? info[0] : defaultInfo}>
        <div class="flex flex-wrap gap-2">
          <${Button} title="Commit this firmware" onclick=${oncommit}
            icon=${Icons.thumbUp} disabled=${disabled} cls="w-full" />
        <//>
      <//>
      <${FirmwareStatus} title="Previous firmware image" info=${info[1] ? info[1] : defaultInfo}>
        <${Button} title="Rollback to this firmware" onclick=${onrollback}
          icon=${Icons.backward} disabled=${disabled} cls="w-full" />
      <//>
      <div class="bg-white xm-4 divide-y border rounded flex flex-col">
        <div class="font-light uppercase flex items-center text-gray-600 px-4 py-2">
          Device control
        <//>
        <div class="px-4 py-3 flex flex-col gap-2 grow">
          <${UploadFileButton}
            title="Upload new firmware: choose .bin file:" publishFn=${publishFn} onupload=${onupload} clean=${clean} setCleanFn=${setClean} disabled=${disabled}
              id=${deviceID} url="api/firmware/upload" accept=".bin,.uf2" />
          <div class="grow"><//>
          <${Button} title="Reboot device" onclick=${onreboot} icon=${Icons.refresh} disabled=${disabled} cls="w-full" />
          <${Button} title="Erase last sector" onclick=${onerase} icon=${Icons.doc} disabled=${disabled} cls="w-full hidden" />
        <//>
      <//>
    <//>
  <//>`;
};

function Config( {deviceData, setDeviceConfig, publishFn} ) {
  const [localConfig, setLocalConfig] = useState();
  const [saveResult, setSaveResult] = useState(null);
  const [saving, setSaving] = useState(false);
  useEffect(() => {
    if (deviceData) {
      if (deviceData.config) {
        setLocalConfig(deviceData.config)
      } else {
        let config = {}
        setLocalConfig(config)
      }
    }
  }, [deviceData]);

  const logOptions = [[0, 'Disable'], [1, 'Error'], [2, 'Info'], [3, 'Debug']];
  const mksetfn = k => (v => setLocalConfig(x => Object.assign({}, x, { [k]: v })));

  const onSave = () => {
    setSaving(true)
    publishFn("config.set", localConfig).then(r => {
      setDeviceConfig(deviceData.id, localConfig)
      setSaveResult("Success!")
      setSaving(false)
    }).catch(e => {
      setDeviceConfig(deviceData.id, deviceData.config)
      setSaveResult("Failed!")
      setSaving(false)
    })
  };

  const onLedToggle = function(ev) {
    localConfig.led_status = !localConfig.led_status;
    onSave();
  };

  if (!deviceData || !localConfig) {
    return ``;
  }

  return html`
  <div class="m-4 space-y-2">
  <h3 class="text-lg font-semibold mb-3">Device ${deviceData.id} Configuration Panel</h3>
  <div class="divide-y border rounded bg-slate-200">
    <div class="bg-slate-100 px-4 py-2 flex items-center font-light uppercase text-gray-600">
      <span class="mr-2">
      Status: ${html`<${Colored} colors=${deviceData.online ? tipColors.green : tipColors.red} text=${deviceData.online ? 'online' : 'offline'} />`}
      </span>
    <//>
    <div class="grid grid-cols-2 gap-1">
      <div class="py-1 divide-y border rounded bg-white flex flex-col">
        <div class="font-light uppercase flex items-center text-gray-600 px-4 py-2">
          LED Settings
        <//>
        <div class="py-2 px-5 flex-1 flex flex-col relative">
        <${Setting} title="LED status" value=${localConfig.led_status} setfn=${onLedToggle} type="switch" disabled=${!deviceData.online} />
        <${Setting} title="LED Pin" type="number" value=${localConfig.led_pin} setfn=${mksetfn('led_pin')} disabled=${!deviceData.online} />
        <//>
      </div>
      <div class="py-1 divide-y border rounded bg-white flex flex-col">
        <div class="font-light uppercase flex items-center text-gray-600 px-4 py-2">
          Log & Display
        <//>
        <div class="py-2 px-5 flex-1 flex flex-col relative">
        <${Setting} title="Log Level" type="select" value=${localConfig.log_level} setfn=${mksetfn('log_level')} options=${logOptions} disabled=${!deviceData.online}/>
        <${Setting} title="Brightness" type="number" value=${localConfig.brightness} setfn=${mksetfn('brightness')} disabled=${!deviceData.online} />
        <div class="mt-3 flex justify-end">
        ${saveResult && html`<${Notification} ok=${saveResult === "Success!"}
        text=${saveResult} close=${() => setSaveResult(null)} />`}
          <${Button} icon=${Icons.save} onclick=${onSave} title=${saving ? "Saving..." : "Save Settings"} disabled=${!deviceData.online || saving} />
        </div>
        <//>
      </div>
    </div>
  </div>
  <${FirmwareUpdate} deviceID=${deviceData.id} publishFn=${publishFn} disabled=${!deviceData.online} info=${[localConfig.crnt_fw, localConfig.prev_fw]} />
</div>`;
}

const App = function() {
  const [devices, setDevices] = useState([]);
  const [currentDevID, setCurrentDevID] = useState(localStorage.getItem('currentDevID') || '');
  const [loading, setLoading] = useState(true);
  const [topic, setTopic] = useState(localStorage.getItem('topic') || default_topic);
  const [error, setError] = useState(null);

  const responseHandlers = useRef({});
  let connSuccessful = false;

  function addResponseHandler(correlationId, handler) {
    responseHandlers[correlationId] = handler;
  }

  function removeResponseHandler(correlationId) {
    delete responseHandlers[correlationId];
  }

  const onRefresh = () => {
    window.location.reload();
  }

  const initConn = () => {
    client = mqtt.connect(url, { 
      connectTimeout: 5000,
      reconnectPeriod: 0
    });

    client.on('connect', () => {
      console.log('Connected to the broker');
      setLoading(false);
      setError(null); // Reset error state upon successful connection
      connSuccessful = true;

      const statusTopic = topic + '/+/status'
      const txTopic = topic + '/+/tx'

      const subscribe = (topic) => {
        client.subscribe(topic, (err) => {
          if (err) {
            console.error('Error subscribing to topic:', err);
            setError('Error subscribing to topic');
          } else {
            console.log('Successfully subscribed to ', topic);
          }
        });
      };

      subscribe(statusTopic)
      subscribe(txTopic)
    });

    client.on('message', (topic, message) => {
      //console.log(`Received message from ${topic}: ${message.toString()}`);
      let response;
      try {
        response = JSON.parse(message.toString());
      } catch (err) {
        console.error(err)
        return;
      }
      
      if (topic.endsWith("/status")) {
        const deviceID = topic.split('/')[1]
        let device = {};
        device.id = deviceID;
        const params = response.params
        if (!params) {
          console.error("Invalid response")
          return
        }
        device.online = params.status === "online"
        if (device.online) {
          device.config = {}
          device.config.led_status = params.led_status
          device.config.led_pin = params.led_pin
          device.config.brightness = params.brightness
          device.config.log_level = params.log_level
          device.config.crnt_fw = params.crnt_fw
          device.config.prev_fw = params.prev_fw
        }
        setDevice(device)
      } else if (topic.endsWith("/tx")) {
        if (!response.id) {
          console.error("Invalid response")
          return
        }
        const handler = responseHandlers[response.id];
        if (handler) {
          handler(response);
          removeResponseHandler(response.id);
        }
      }
    });

    client.on('error', (err) => {
      console.error('Connection error:', err);
      setError('Connection cannot be established.');
    });

    client.on('close', () => {
      if (!connSuccessful) {
        console.error('Failed to connect to the broker.');
        setError('Connection cannot be established.');
        setLoading(false);
      }
    });
  };

  useEffect(() => {
    initConn();
  }, []);

  const handlePublish = (methodName, parameters, timeout = 5000) => {
    return new Promise((resolve, reject) => {
      const randomIdGenerator = function(length) {
        return Math.random().toString(36).substring(2, length+2);
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
        const rpcPayload = {
          method: methodName,
          id: randomID
        };
  
        if (parameters) {
          rpcPayload.params = parameters;
        }
  
        client.publish(rxTopic, JSON.stringify(rpcPayload));
      }
    });
  };

  const getDeviceByID = (deviceID) => {
    return devices.find(d => d.id === deviceID);
  }

  const setDevice = (devData) => {
    setDevices(prevDevices => {
      const devIndex = prevDevices.findIndex(device => device.id === devData.id);
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
          return {
            ...device,
            config: config
          };
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
  }

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
            <${Button} title="Retry" onclick=${onRefresh} icon=${Icons.refresh} class="absolute top-4 right-4" />
          </div>
        </div>
      </div>
    </div>`;
  }

  return html`
  <div class="min-h-screen bg-slate-100 flex flex-col space-y-2">
    <${Header} topic=${topic} setTopicFn=${setTopic}/>
    <div class="transition-all duration-300 transform flex flex-grow gap-4">
      <div class="flex-none bg-white rounded shadow ">
        <${Devices} devices=${devices} onClickFn=${onDeviceClick} />
      <//>
      <div class="flex-1 flex-grow bg-white rounded shadow h-full">
        <${Config} deviceData=${getDeviceByID(currentDevID)} setDeviceConfig=${setDeviceConfig} publishFn=${handlePublish}/>
      <//>
    <//>
  <//>`;
};

window.onload = () => render(h(App), document.body);
