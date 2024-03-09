//  NOTE: API calls must start with 'api/' in order to serve the app at any URI

'use strict';
import { h, render, useState, useEffect, useRef, html, Router } from  './bundle.js';
import { Icons, Login, Setting, Button, Stat, tipColors, Colored, Notification, Pagination, UploadFileButton } from './components.js';

const Logo = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" viewBox="0 0 12.87 12.85"><defs><style>.ll-cls-1{fill:none;stroke:#000;stroke-miterlimit:10;stroke-width:0.5px;}</style></defs><g id="Layer_2" data-name="Layer 2"><g id="Layer_1-2" data-name="Layer 1"><path class="ll-cls-1" d="M12.62,1.82V8.91A1.58,1.58,0,0,1,11,10.48H4a1.44,1.44,0,0,1-1-.37A.69.69,0,0,1,2.84,10l-.1-.12a.81.81,0,0,1-.15-.48V5.57a.87.87,0,0,1,.86-.86H4.73V7.28a.86.86,0,0,0,.86.85H9.42a.85.85,0,0,0,.85-.85V3.45A.86.86,0,0,0,10.13,3,.76.76,0,0,0,10,2.84a.29.29,0,0,0-.12-.1,1.49,1.49,0,0,0-1-.37H2.39V1.82A1.57,1.57,0,0,1,4,.25H11A1.57,1.57,0,0,1,12.62,1.82Z"/><path class="ll-cls-1" d="M10.48,10.48V11A1.58,1.58,0,0,1,8.9,12.6H1.82A1.57,1.57,0,0,1,.25,11V3.94A1.57,1.57,0,0,1,1.82,2.37H8.9a1.49,1.49,0,0,1,1,.37l.12.1a.76.76,0,0,1,.11.14.86.86,0,0,1,.14.47V7.28a.85.85,0,0,1-.85.85H8.13V5.57a.86.86,0,0,0-.85-.86H3.45a.87.87,0,0,0-.86.86V9.4a.81.81,0,0,0,.15.48l.1.12a.69.69,0,0,0,.13.11,1.44,1.44,0,0,0,1,.37Z"/></g></g></svg>`;

function Header({logout, user, setShowSidebar, showSidebar}) {
  return html`
<div class="bg-white sticky top-0 z-[48] xw-full border-b py-2 ${showSidebar && 'pl-72'} transition-all duration-300 transform">
  <div class="px-2 w-full py-0 my-0 flex items-center">
    <button type="button" onclick=${ev => setShowSidebar(v => !v)} class="text-slate-400">
      <${Icons.bars3} class="h-6" />
    <//>
    <div class="flex flex-1 gap-x-4 self-stretch lg:gap-x-6">
      <div class="relative flex flex-1"><//>
      <div class="flex items-center gap-x-4 lg:gap-x-6">
        <span class="text-sm text-slate-400">logged in as: ${user}<//>
        <div class="hidden lg:block lg:h-4 lg:w-px lg:bg-gray-200" aria-hidden="true"><//>
        <${Button} title="Logout" icon=${Icons.logout} onclick=${logout} />
      <//>
    <//>
  <//>
<//>`;
};

function Sidebar({url, show}) {
  const NavLink = ({title, icon, href, url}) => html`
  <div>
    <a href="#${href}" class="${href == url ? 'bg-slate-50 text-blue-600 group' : 'text-gray-700 hover:text-blue-600 hover:bg-gray-50 group'} flex gap-x-3 rounded-md p-2 text-sm leading-6 font-semibold">
      <${icon} class="w-6 h-6"/>
      ${title}
    <///>
  <//>`;
  return html`
<div class="bg-violet-100 hs-overlay hs-overlay-open:translate-x-0
            -translate-x-full transition-all duration-300 transform
            fixed top-0 left-0 bottom-0 z-[60] w-72 bg-white border-r
            border-gray-200 overflow-y-auto scrollbar-y
            ${show && 'translate-x-0'} right-auto bottom-0">
  <div class="flex flex-col m-4 gap-y-6">
    <div class="flex h-10 shrink-0 items-center gap-x-4 font-bold text-xl text-slate-500">
      <${Logo} class="h-full"/> Your Brand
    <//>
    <div class="flex flex-1 flex-col">
      <${NavLink} title="Dashboard" icon=${Icons.home} href="/" url=${url} />
      <${NavLink} title="Settings" icon=${Icons.cog} href="/settings" url=${url} />
      <${NavLink} title="Firmware Update" icon=${Icons.download} href="/update" url=${url} />
      <${NavLink} title="Events" icon=${Icons.alert} href="/events" url=${url} />
    <//>
  <//>
<//>`;
};

function Events({}) {
  const [events, setEvents] = useState([]);
  const [page, setPage] = useState(1);

  const refresh = () =>
    fetch('api/events/get', {
        method: 'POST', body: JSON.stringify({page: page}),
      }).then(r => r.json())
        .then(r => setEvents(r));

  useEffect(refresh, [page]);

  useEffect(() => {
    setPage(JSON.parse(localStorage.getItem('page')));
      }, []);

  useEffect(() => {
    localStorage.setItem('page', page.toString());
  }, [page]);

  const Th = props => html`<th scope="col" class="sticky top-0 z-10 border-b border-slate-300 bg-white bg-opacity-75 py-1.5 px-4 text-left text-sm font-semibold text-slate-900 backdrop-blur backdrop-filter">${props.title}</th>`;
  const Td = props => html`<td class="whitespace-nowrap border-b border-slate-200 py-2 px-4 pr-3 text-sm text-slate-900">${props.text}</td>`;
  const Prio = ({prio}) => {
    const text = ['high', 'medium', 'low'][prio];
    const colors = [tipColors.red, tipColors.yellow, tipColors.green][prio];
    return html`<${Colored} colors=${colors} text=${text} />`;
  };

  const Event = ({e}) => html`
<tr>
  <${Td} text=${['power', 'hardware', 'tier3', 'tier4'][e.type]} />
  <${Td} text=${html`<${Prio} prio=${e.prio}/>`} />
  <${Td} text=${e.time ? (new Date(e.time * 1000)).toLocaleString() : '1970-01-01'} />
  <${Td} text=${e.text} />
<//>`;

return html`
<div class="m-4 divide-y divide-gray-200 overflow-auto rounded bg-white">
  <div class="font-semibold flex items-center text-gray-600 px-3 justify-between whitespace-nowrap">
    <div class="font-semibold flex items-center text-gray-600">
      <div class="mr-4">EVENT LOG</div>
    </div>
    <${Pagination} currentPage=${page} setPageFn=${setPage} totalItems=400 itemsPerPage=20 />
  <//>
  <div class="inline-block min-w-full align-middle" style="max-height: 82vh; overflow: auto;">
    <table class="min-w-full border-separate border-spacing-0">
      <thead>
        <tr>
          <${Th} title="Type" />
          <${Th} title="Prio" />
          <${Th} title="Time" />
          <${Th} title="Description" />
        </tr>
      </thead>
      <tbody>
        ${(events.arr ? events.arr : []).map(e => h(Event, {e}))}
      </tbody>
    </table>
  <//>
<//>`;
};

function Chart({data}) {
  const n = data.length /* entries */, w = 20 /* entry width */, ls = 15/* left space */;
  const h = 100 /* graph height */, yticks = 5 /* Y axis ticks */, bs = 10 /* bottom space */;
  const ymax = 25;
  const yt = i => (h - bs) / yticks * (i + 1);
  const bh = p => (h - bs) * p / 100; // Bar height
  const by = p => (h - bs) - bh(p);
  const range = (start, size, step) => Array.from({length: size}, (_, i) => i * (step || 1) + start);
  // console.log(ds);
  return html`
<div class="my-4 divide-y divide-gray-200 overflow-auto rounded bg-white">
  <div class="font-light uppercase flex items-center text-gray-600 px-4 py-2">
  Temperature, last 24h
  <//>
  <div class="relative">
    <svg class="bg-yellow-x50 w-full p-4" viewBox="0 0 ${n*w+ls} ${h}">
      ${range(0, yticks).map(i => html`
        <line x1=0 y1=${yt(i)} x2=${ls+n*w} y2=${yt(i)} stroke-width=0.3 class="stroke-slate-300" stroke-dasharray="1,1" />
        <text x=0 y=${yt(i)-2} class="text-[6px] fill-slate-400">${ymax-ymax/yticks*(i+1)}<//>
      `)}
      ${range(0, n).map(x => html`
        <rect x=${ls+x*w} y=${by(data[x]*100/ymax)} width=12 height=${bh(data[x]*100/ymax)} rx=2 class="fill-cyan-500" />
        <text x=${ls+x*w} y=100 class="text-[6px] fill-slate-400">${x*2}:00<//>
      `)}
    <//>
  <//>
<//>`;
};

function DeveloperNote({text, children}) {
  return html`
<div class="flex p-4 gap-2">
  <div class="text-sm text-slate-500">
    <div class="flex items-center">
      <${Icons.info} class="self-start basis-[30px] grow-0 shrink-0 text-green-600 mr-2" />
      <div class="font-semibold">Developer Note<//>
    <//>
    ${(text || '').split('.').map(v => html` <p class="my-2 ">${v}<//>`)}
    ${children}
  <//>
<//>`;
};

function Main({}) {
  const [stats, setStats] = useState(null);
  const refresh = () => fetch('api/stats/get').then(r => r.json()).then(r => setStats(r));
  useEffect(refresh, []);
  if (!stats) return '';
  return html`
<div class="p-2">
  <div class="p-4 sm:p-2 mx-auto grid grid-cols-2 lg:grid-cols-4 gap-4">
    <${Stat} title="Temperature" text="${stats.temperature} °C" tipText="good" tipIcon=${Icons.ok} tipColors=${tipColors.green} />
    <${Stat} title="Humidity" text="${stats.humidity} %" tipText="warn" tipIcon=${Icons.warn} tipColors=${tipColors.yellow} />
    <div class="bg-white col-span-2 border rounded-md shadow-lg" role="alert">
      <${DeveloperNote} text="Stats data is received from the Mongoose backend" />
    <//>
  <//>
  <div class="p-4 sm:p-2 mx-auto grid grid-cols-1 lg:grid-cols-2 gap-4">

    <${Chart} data=${stats.points} />

    <div class="my-4 hx-24 bg-white border rounded-md shadow-lg" role="alert">
      <${DeveloperNote}
        text="This chart is an SVG image, generated on the fly from the
        data returned by the api/stats/get API call" />
    <//>
  <//>
<//>`;
};

function FirmwareStatus({title, info, children}) {
  const state = ['UNAVAILABLE', 'FIRST_BOOT', 'NOT_COMMITTED', 'COMMITTED'][(info.status || 0) % 4];
  const valid = info.status > 0;
  return html`
<div class="bg-white py-1 divide-y border rounded">
  <div class="font-light uppercase flex items-center text-gray-600 px-4 py-2">
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


function FirmwareUpdate({}) {
  const [info, setInfo] = useState([{}, {}]);
  const refresh = () => fetch('api/firmware/status').then(r => r.json()).then(r => setInfo(r));
  useEffect(refresh, []);
  const oncommit = ev => fetch('api/firmware/commit')
    .then(r => r.json())
    .then(refresh);
  const onreboot = ev => fetch('api/device/reset')
    .then(r => r.json())
    .then(r => new Promise(r => setTimeout(ev => { refresh(); r(); }, 3000)));
  const onrollback = ev => fetch('api/firmware/rollback')
    .then(onreboot);
  const onerase = ev => fetch('api/device/eraselast').then(refresh);
  const onupload = function(ok, name, size) {
    if (!ok) return false;
    return new Promise(r => setTimeout(ev => { refresh(); r(); }, 3000));
  };
  return html`
<div class="m-4 gap-4 grid grid-cols-1 lg:grid-cols-3">
  <${FirmwareStatus} title="Current firmware image" info=${info[0]}>
    <div class="flex flex-wrap gap-2">
      <${Button} title="Commit this firmware" onclick=${oncommit}
        icon=${Icons.thumbUp} disabled=${info[0].status == 3} cls="w-full" />
    <//>
  <//>
  <${FirmwareStatus} title="Previous firmware image" info=${info[1]}>
    <${Button} title="Rollback to this firmware" onclick=${onrollback}
      icon=${Icons.backward} disabled=${info[1].status == 0} cls="w-full" />
  <//>
  <div class="bg-white xm-4 divide-y border rounded flex flex-col">
    <div class="font-light uppercase flex items-center text-gray-600 px-4 py-2">
      Device control
    <//>
    <div class="px-4 py-3 flex flex-col gap-2 grow">
      <${UploadFileButton}
        title="Upload new firmware .bin file" onupload=${onupload}
      url="api/firmware/upload" accept=".bin,.uf2" />
      <div class="grow"><//>
      <${Button} title="Reboot device" onclick=${onreboot} icon=${Icons.refresh} cls="w-full" />
      <${Button} title="Erase last sector" onclick=${onerase} icon=${Icons.doc} cls="w-full hidden" />
    <//>
  <//>
<//>


<div class="m-4 gap-4 grid grid-cols-1 lg:grid-cols-2">
  <div class="bg-white border shadow-lg">
    <${DeveloperNote}>
      <div class="my-2">
        Firmware status and other information is stored in the last sector
        of flash
      <//>
      <div class="my-2">
        Firmware status can be FIRST_BOOT, UNCOMMITTED or COMMITTED. If no
        information is available, it is UNAVAILABLE.
      <//>
      <div class="my-2">  
        This GUI loads a firmware file and sends it chunk by chunk to the
        device, passing current chunk offset, total firmware size and a file name:
        api/firmware/upload?offset=X&total=Y&name=Z
      <//>
    <//>
  <//>

  <div class="bg-white border shadow-lg">
    <${DeveloperNote}>
      <div>
        Firmware update mechanism defines 3 API functions that the target
        device must implement: mg_ota_begin(), mg_ota_write() and mg_ota_end()
      <//>
      <div class="my-2">  
        RESTful API handlers use ota_xxx() API to save firmware to flash.
        The last 0-length chunk triggers ota_end() which performs firmware
        update using saved firmware image
      <//>
      <div class="my-2">  
        <a class="link text-blue-600 underline" 
          href="https://mongoose.ws/webinars/">Join our free webinar</a> to
        get detailed explanations about possible firmware updates strategies
        and implementation demo
      <//>
    <//>
  <//>

<//>`;
};

function Settings({}) {
  const [settings, setSettings] = useState(null);
  const [saveResult, setSaveResult] = useState(null);
  const refresh = () => fetch('api/settings/get')
    .then(r => r.json())
    .then(r => setSettings(r));
  useEffect(refresh, []);

  const mksetfn = k => (v => setSettings(x => Object.assign({}, x, {[k]: v}))); 
  const onsave = ev => fetch('api/settings/set', {
    method: 'post', body: JSON.stringify(settings) 
  }).then(r => r.json())
    .then(r => setSaveResult(r))
    .then(refresh);

  if (!settings) return '';
  const logOptions = [[0, 'Disable'], [1, 'Error'], [2, 'Info'], [3, 'Debug']];
  return html`
<div class="m-4 grid grid-cols-1 gap-4 md:grid-cols-2">

  <div class="py-1 divide-y border rounded bg-white flex flex-col">
    <div class="font-light uppercase flex items-center text-gray-600 px-4 py-2">
      Device Settings
    <//>
    <div class="py-2 px-5 flex-1 flex flex-col relative">
      ${saveResult && html`<${Notification} ok=${saveResult.status}
        text=${saveResult.message} close=${() => setSaveResult(null)} />`}

      <${Setting} title="Enable Logs" value=${settings.log_enabled} setfn=${mksetfn('log_enabled')} type="switch" />
      <${Setting} title="Log Level" value=${settings.log_level} setfn=${mksetfn('log_level')} type="select" addonLeft="0-3" disabled=${!settings.log_enabled} options=${logOptions}/>
      <${Setting} title="Brightness" value=${settings.brightness} setfn=${mksetfn('brightness')} type="number" addonRight="%" />
      <${Setting} title="Device Name" value=${settings.device_name} setfn=${mksetfn('device_name')} type="" />
      <div class="mb-1 mt-3 flex place-content-end"><${Button} icon=${Icons.save} onclick=${onsave} title="Save Settings" /><//>
    <//>
  <//>

  <div class="bg-white border rounded-md text-ellipsis overflow-auto" role="alert">
    <${DeveloperNote}
        text="A variety of controls are pre-defined to ease the development:
          toggle button, dropdown select, input field with left and right
          addons. Device settings are received by calling
          api/settings/get API call, which returns settings JSON object.
          Clicking on the save button calls api/settings/set
          API call" />
  <//>

<//>`;
};

const App = function({}) {
  const [loading, setLoading] = useState(true);
  const [url, setUrl] = useState('/');
  const [user, setUser] = useState('');
  const [showSidebar, setShowSidebar] = useState(true);

  const logout = () => fetch('api/logout').then(r => setUser(''));
  const login = r => !r.ok ? setLoading(false) && setUser(null) : r.json()
      .then(r => setUser(r.user))
      .finally(r => setLoading(false));

  useEffect(() => fetch('api/login').then(login), []);

  if (loading) return '';  // Show blank page on initial load
  if (!user) return html`<${Login} loginFn=${login} logoIcon=${Logo}
    title="Device Dashboard Login" 
    tipText="To login, use: admin/admin, user1/user1, user2/user2" />`; // If not logged in, show login screen

  return html`
<div class="min-h-screen bg-slate-100">
  <${Sidebar} url=${url} show=${showSidebar} />
  <${Header} logout=${logout} user=${user} showSidebar=${showSidebar} setShowSidebar=${setShowSidebar} />
  <div class="${showSidebar && 'pl-72'} transition-all duration-300 transform">
    <${Router} onChange=${ev => setUrl(ev.url)} history=${History.createHashHistory()} >
      <${Main} default=${true} />
      <${Settings} path="settings" />
      <${FirmwareUpdate} path="update" />
      <${Events} path="events" />
    <//>
  <//>
<//>`;
};

window.onload = () => render(h(App), document.body);
