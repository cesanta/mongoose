'use strict';
import { h, render, useState, useEffect, html, Router } from  './bundle.js';
import { Icons, Login, Setting, Button, Stat, tipColors, Colored, Notification } from './components.js';

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

const NavLink = ({title, icon, href, url}) => html`
<div>
  <a href="#${href}" class="${href == url ? 'bg-slate-50 text-blue-600 group' : 'text-gray-700 hover:text-blue-600 hover:bg-gray-50 group'} flex gap-x-3 rounded-md p-2 text-sm leading-6 font-semibold">
    <${icon} class="w-6 h-6"/>
    ${title}
  <///>
<//>`;

function Sidebar({url, show}) {
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
      <${NavLink} title="Connected Devices" icon=${Icons.link} href="/devices" url=${url} />
      <${NavLink} title="Firewall" icon=${Icons.shield} href="/firewall" url=${url} />
      <${NavLink} title="DHCP" icon=${Icons.barsdown} href="/dhcp" url=${url} />
      <${NavLink} title="Administration" icon=${Icons.settings} href="/admin" url=${url} />
    <//>
  <//>
<//>`;
};

function Events({}) {
  const [events, setEvents] = useState([]);
  const refresh = () => fetch('api/events/get').then(r => r.json()).then(r => setEvents(r)).catch(e => console.log(e));
  useEffect(refresh, []);

  const Th = props => html`<th scope="col" class="sticky top-0 z-10 border-b border-slate-300 bg-white bg-opacity-75 py-1.5 px-4 text-left text-sm font-semibold text-slate-900 backdrop-blur backdrop-filter">${props.title}</th>`;
  const Td = props => html`<td class="whitespace-nowrap border-b border-slate-200 py-2 px-4 pr-3 text-sm text-slate-900">${props.text}</td>`;
  const Prio = ({prio}) => {
    const text = ['high', 'medium', 'low'][prio];
    const colors = [tipColors.red, tipColors.yellow, tipColors.green][prio];
    return html`<${Colored} colors=${colors} text=${text} />`;
  };
  const Event = ({e}) => html`
<tr>
  <${Td} text=${['network', 'optic', 'power', 'routing'][e.type]} />
  <${Td} text=${html`<${Prio} prio=${e.prio}/>`} />
  <${Td} text=${e.time || '1970-01-01'} />
  <${Td} text=${e.text} />
<//>`;
  //console.log(events);

  return html`
<div class="m-4 h-64 divide-y divide-gray-200 overflow-auto rounded bg-white">
  <div class="font-light uppercase flex items-center text-slate-600 px-4 py-2">
    Event Log
  <//>
  <div class="inline-block min-w-full py-2 align-middle">
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
        ${events.map(e => h(Event, {e}))}
      </tbody>
    </table>
  <//>
<//>`;
};

function ModemStatus({}) {
  const th = props => html`<th scope="col" class="px-4 py-3 text-xs text-left font-light uppercase text-gray-800"> ${props.title} <//>`;
  const td = props => html`<td scope="col" class="px-4 py-3 text-sm text-left font-normal text-gray-800"> ${props.text} <//>`;
  const tr = props => html`<tr class="hover:bg-slate-50">${props.cols.map(text => h(td, {text}))}<//>`;

  return html`
<div class="m-4 divide-y divide-gray-200 overflow-auto rounded bg-white">
  <div class="font-light uppercase flex items-center text-gray-600 px-4 py-2">
    Cable Modem Status
  <//>
  <table class="min-w-full divide-y divide-gray-200 dark:divide-gray-700">
    <thead class="">
      <tr>
        <${th} title="" />
        <${th} title="Status" />
        <${th} title="Comment" />
      </tr>
    </thead>
    <tbody>
      <${tr} cols=${['Downstream Channel (Hz)', '4820000000', 'Locked']} />
      <${tr} cols=${['Upstream Channel (Hz)', '3280000', 'Ranged']} />
      <${tr} cols=${['Provisioning State', 'Online', 'Operational']} />
    </tbody>
  <//>
<//>`;
};

const range = (start, size, step) => Array.from({length: size}, (_, i) => i * (step || 1) + start);

function SpeedChart({stats}) {
  const us = stats.upload_speed, ds = stats.download_speed;
  const n = us.length /* entries */, w = 20 /* entry width */, ls = 15/* left space */;
  const h = 100 /* graph height */, yticks = 5 /* Y axis ticks */, bs = 10 /* bottom space */;
  const ymax = 500;
  const yt = i => (h - bs) / yticks * (i + 1);
  const bh = p => (h - bs) * p / 100; // Bar height
  const by = p => (h - bs) - bh(p);
  // console.log(ds);
  return html`
<div class="m-4 divide-y divide-gray-200 overflow-auto rounded bg-white">
  <div class="font-light uppercase flex items-center text-gray-600 px-4 py-2">
    Internet Speed, last 24h
  <//>
  <div class="relative">
    <svg class="bg-yellow-x50 w-full p-4" viewBox="0 0 ${n*w+ls} ${h}">
      ${range(0, yticks).map(i => html`
        <line x1=0 y1=${yt(i)} x2=${ls+n*w} y2=${yt(i)} stroke-width=0.3 class="stroke-slate-300" stroke-dasharray="1,1" />
        <text x=0 y=${yt(i)-2} class="text-[6px] fill-slate-400">${ymax-ymax/yticks*(i+1)}<//>
      `)}
      ${range(0, n).map(x => html`
        <rect x=${ls+x*w} y=${by(ds[x]*100/ymax)} width=8 height=${bh(ds[x]*100/ymax)} rx=2 class="fill-sky-800" />
        <rect x=${ls+x*w+8} y=${by(us[x]*100/ymax)} width=8 height=${bh(us[x]*100/ymax)} rx=2 class="fill-sky-400" />
        <text x=${ls+x*w} y=100 class="text-[6px] fill-slate-400">${x*2}:00<//>
      `)}
    <//>
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
  <div class="max-w-[85rem] px-4 sm:px-2 lg:px-4 lg:py-4 mx-auto">
    <div class="grid sm:grid-cols-2 lg:grid-cols-4 gap-4 sm:gap-6">
      <${Stat} title="Download Speed" text="${stats.download_speed[0]} Mbps" tipText="slow" tipIcon=${Icons.alert} tipColors=${tipColors.red} />
      <${Stat} title="Upload Speed" text="${stats.upload_speed[0]} Mbps" tipText="good" tipIcon=${Icons.ok} tipColors=${tipColors.green} />
      <${Stat} title="Daily Downloaded" text="${stats.downloaded} Mb" tipText="-2.2%" tipIcon=${Icons.arrowdown} />
      <${Stat} title="Daily Uploaded" text="${stats.uploaded} Mb" tipText="+12.7%" tipIcon=${Icons.arrowup} />
    <//>
  <//>
  <div class="grid grid-cols-1 lg:grid-cols-2 gap-2">
    <${SpeedChart} stats=${stats} />
    <${Events} />
    <${ModemStatus} />
  <//>
<//>`;
};

function Devices({}) {
  const [devices, setDevices] = useState(null);
  const refresh = () => fetch('api/devices/get')
    .then(r => r.json())
    .then(r => setDevices(r));
  useEffect(refresh, []);

  const th = props => html`<th scope="col" class="sticky top-0 z-10 border-b border-slate-300 bg-white bg-opacity-75 py-1.5 px-4 text-left text-sm font-semibold text-slate-900 backdrop-blur backdrop-filter">${props.title}</th>`;
  const td = props => html`<td class="whitespace-nowrap border-b border-slate-200 py-2 px-4 pr-3 text-sm text-slate-900">${props.text}</td>`;

  const Device = ({d}) => html`
<tr>
  <${td} text=${d.dev_name} />
  <${td} text=${d.mac} />
  <${td} text=${d.ip} />
  <${td} text=${d.speed} />
  <${td} text=${d.connected_to} />
  <${td} text=${d.lease_time_left} />
  <${td} text=${d.last_seen} />
<//>`;

if (!devices) return '';
return html`
  <div class="overflow-x-auto shadow-xl rounded-xl bg-white dark:bg-gray-800 my-3 mx-4">
    <div class="font-light uppercase flex text-slate-600 px-4 py-2 justify-between">
      Connected devices
      <${Button} title="Refresh table" icon=${Icons.refresh} onclick=${refresh} mx-2 />
    <//>
    <div class="align-middle inline-block w-full">
      <div class="overflow-hidden">
        <table class="min-w-full divide-y divide-gray-200 dark:divide-gray-700 text-sm md:text-base lg:text-lg">
          <thead class="bg-gray-200 dark:bg-gray-700 text-gray-600 dark:text-gray-300">
            <tr>
              <${th} title="Device Name" />
              <${th} title="MAC address" />
              <${th} title="IP address" />
              <${th} title="Speed (Mbps)" />
              <${th} title="Connected to" />
              <${th} title="Lease time left" />
              <${th} title="Last seen" />
            </tr>
          </thead>
          <tbody class="bg-white dark:bg-gray-900 text-gray-800 dark:text-gray-200">
            ${devices.map(d => h(Device, {d}))}
          </tbody>
        </table>
      </div>
    </div>
  </div>`;
};

function DHCP({}) {
  const [dhcp, setDhcp] = useState(null);
  const [saveResult, setSaveResult] = useState(null);
  const refresh = () => fetch('api/dhcp/get')
    .then(r => r.json())
    .then(r => setDhcp(r));
  useEffect(refresh, []);

  const mksetfn = k => (v => setDhcp(x => Object.assign({}, x, {[k]: v}))); 
  const onsave = ev => fetch('api/dhcp/set', {
    method: 'post', body: JSON.stringify(dhcp) 
  }).then(r => r.json())
    .then(r => setSaveResult(r))
    .then(refresh);

  if (!dhcp) return '';
  return html`
<div class="m-4 grid grid-cols-1 gap-4 md:grid-cols-2">

  <div class="py-1 divide-y border rounded bg-white flex flex-col">
    <div class="font-light uppercase flex items-center text-gray-600 px-4 py-2">
      DHCP Server Settings
    <//>
    <div class="py-2 px-5 flex-1 flex flex-col relative">
      ${saveResult && html`<${Notification} ok=${saveResult.status}
        text=${saveResult.message} close=${() => setSaveResult(null)} />`}

      <${Setting} title="Enabled" value=${dhcp.enabled} setfn=${mksetfn('enabled')} type="switch" />
      <${Setting} title="Start Address" value=${dhcp.address_begin} setfn=${mksetfn('address_begin')} type="number" addonLeft="192.168.0." disabled=${!dhcp.enabled} />
      <${Setting} title="End Address" value=${dhcp.address_end} setfn=${mksetfn('address_end')} type="number" addonLeft="192.168.0." disabled=${!dhcp.enabled} />
      <${Setting} title="Lease Time" value=${dhcp.lease_time_sec} setfn=${mksetfn('lease_time_sec')} type="number" addonRight="seconds" disabled=${!dhcp.enabled} />
      <div class="mb-1 mt-3 flex place-content-end"><${Button} icon=${Icons.save} onclick=${onsave} title="Save Settings" /><//>
    <//>
  <//>
<//>`;
};

const App = function({}) {
  const [loading, setLoading] = useState(true);
  const [url, setUrl] = useState('/');
  const [user, setUser] = useState('');
  const [showSidebar, setShowSidebar] = useState(true);

  const logout = () => fetch('api/logout').then(r => setUser(''));
  const login = r => !r.ok ? setLoading(false) : r.json()
      .then(r => setUser(r.user))
      .finally(r => setLoading(false));

  useEffect(() => fetch('api/login').then(login), []);

  if (loading) return '';  // Show blank page on initial load
  if (!user) return html`<${Login} loginFn=${login} logoIcon=${Logo} title="WiFi Router Login" tipText="To login, use: admin/admin, user1/user1, user2/user2" />`; // If not logged in, show login screen

  return html`
<div class="min-h-screen bg-slate-100">
  <${Sidebar} url=${url} show=${showSidebar} />
  <${Header} logout=${logout} user=${user} showSidebar=${showSidebar} setShowSidebar=${setShowSidebar} />
  <div class="${showSidebar && 'pl-72'} transition-all duration-300 transform">
    <${Router} onChange=${ev => setUrl(ev.url)} history=${History.createHashHistory()} >
      <${Main} default=${true} />
      <${DHCP} path="dhcp" />
      <${Devices} path="devices" />
    <//>
  <//>
<//>`;
};

window.onload = ev => render(h(App), document.body);
