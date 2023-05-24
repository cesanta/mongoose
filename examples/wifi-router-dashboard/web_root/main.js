'use strict';
import { h, render, useState, useEffect, useRef, html, Router } from  './bundle.js';

const Logo = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" viewBox="0 0 12.87 12.85"><defs><style>.ll-cls-1{fill:none;stroke:#000;stroke-miterlimit:10;stroke-width:0.5px;}</style></defs><g id="Layer_2" data-name="Layer 2"><g id="Layer_1-2" data-name="Layer 1"><path class="ll-cls-1" d="M12.62,1.82V8.91A1.58,1.58,0,0,1,11,10.48H4a1.44,1.44,0,0,1-1-.37A.69.69,0,0,1,2.84,10l-.1-.12a.81.81,0,0,1-.15-.48V5.57a.87.87,0,0,1,.86-.86H4.73V7.28a.86.86,0,0,0,.86.85H9.42a.85.85,0,0,0,.85-.85V3.45A.86.86,0,0,0,10.13,3,.76.76,0,0,0,10,2.84a.29.29,0,0,0-.12-.1,1.49,1.49,0,0,0-1-.37H2.39V1.82A1.57,1.57,0,0,1,4,.25H11A1.57,1.57,0,0,1,12.62,1.82Z"/><path class="ll-cls-1" d="M10.48,10.48V11A1.58,1.58,0,0,1,8.9,12.6H1.82A1.57,1.57,0,0,1,.25,11V3.94A1.57,1.57,0,0,1,1.82,2.37H8.9a1.49,1.49,0,0,1,1,.37l.12.1a.76.76,0,0,1,.11.14.86.86,0,0,1,.14.47V7.28a.85.85,0,0,1-.85.85H8.13V5.57a.86.86,0,0,0-.85-.86H3.45a.87.87,0,0,0-.86.86V9.4a.81.81,0,0,0,.15.48l.1.12a.69.69,0,0,0,.13.11,1.44,1.44,0,0,0,1,.37Z"/></g></g></svg>`;

const IconHeart = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M4.318 6.318a4.5 4.5 0 000 6.364L12 20.364l7.682-7.682a4.5 4.5 0 00-6.364-6.364L12 7.636l-1.318-1.318a4.5 4.5 0 00-6.364 0z"></path></svg>`;
const IconDownArrowBox = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M9 8.25H7.5a2.25 2.25 0 00-2.25 2.25v9a2.25 2.25 0 002.25 2.25h9a2.25 2.25 0 002.25-2.25v-9a2.25 2.25 0 00-2.25-2.25H15M9 12l3 3m0 0l3-3m-3 3V2.25" /> </svg>`;
const IconUpArrowBox = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M9 8.25H7.5a2.25 2.25 0 00-2.25 2.25v9a2.25 2.25 0 002.25 2.25h9a2.25 2.25 0 002.25-2.25v-9a2.25 2.25 0 00-2.25-2.25H15m0-3l-3-3m0 0l-3 3m3-3V15" /> </svg>`;
const IconSettings = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M9.594 3.94c.09-.542.56-.94 1.11-.94h2.593c.55 0 1.02.398 1.11.94l.213 1.281c.063.374.313.686.645.87.074.04.147.083.22.127.324.196.72.257 1.075.124l1.217-.456a1.125 1.125 0 011.37.49l1.296 2.247a1.125 1.125 0 01-.26 1.431l-1.003.827c-.293.24-.438.613-.431.992a6.759 6.759 0 010 .255c-.007.378.138.75.43.99l1.005.828c.424.35.534.954.26 1.43l-1.298 2.247a1.125 1.125 0 01-1.369.491l-1.217-.456c-.355-.133-.75-.072-1.076.124a6.57 6.57 0 01-.22.128c-.331.183-.581.495-.644.869l-.213 1.28c-.09.543-.56.941-1.11.941h-2.594c-.55 0-1.02-.398-1.11-.94l-.213-1.281c-.062-.374-.312-.686-.644-.87a6.52 6.52 0 01-.22-.127c-.325-.196-.72-.257-1.076-.124l-1.217.456a1.125 1.125 0 01-1.369-.49l-1.297-2.247a1.125 1.125 0 01.26-1.431l1.004-.827c.292-.24.437-.613.43-.992a6.932 6.932 0 010-.255c.007-.378-.138-.75-.43-.99l-1.004-.828a1.125 1.125 0 01-.26-1.43l1.297-2.247a1.125 1.125 0 011.37-.491l1.216.456c.356.133.751.072 1.076-.124.072-.044.146-.087.22-.128.332-.183.582-.495.644-.869l.214-1.281z" /> <path stroke-linecap="round" stroke-linejoin="round" d="M15 12a3 3 0 11-6 0 3 3 0 016 0z" /> </svg>`;
const IconDesktop = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M9 17.25v1.007a3 3 0 01-.879 2.122L7.5 21h9l-.621-.621A3 3 0 0115 18.257V17.25m6-12V15a2.25 2.25 0 01-2.25 2.25H5.25A2.25 2.25 0 013 15V5.25m18 0A2.25 2.25 0 0018.75 3H5.25A2.25 2.25 0 003 5.25m18 0V12a2.25 2.25 0 01-2.25 2.25H5.25A2.25 2.25 0 013 12V5.25" /> </svg>`;
const IconScan = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M7.5 3.75H6A2.25 2.25 0 003.75 6v1.5M16.5 3.75H18A2.25 2.25 0 0120.25 6v1.5m0 9V18A2.25 2.25 0 0118 20.25h-1.5m-9 0H6A2.25 2.25 0 013.75 18v-1.5M15 12a3 3 0 11-6 0 3 3 0 016 0z" /> </svg> `;
const IconAlert = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M14.857 17.082a23.848 23.848 0 005.454-1.31A8.967 8.967 0 0118 9.75v-.7V9A6 6 0 006 9v.75a8.967 8.967 0 01-2.312 6.022c1.733.64 3.56 1.085 5.455 1.31m5.714 0a24.255 24.255 0 01-5.714 0m5.714 0a3 3 0 11-5.714 0M3.124 7.5A8.969 8.969 0 015.292 3m13.416 0a8.969 8.969 0 012.168 4.5" /> </svg>`;
const IconRefresh = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M16.023 9.348h4.992v-.001M2.985 19.644v-4.992m0 0h4.992m-4.993 0l3.181 3.183a8.25 8.25 0 0013.803-3.7M4.031 9.865a8.25 8.25 0 0113.803-3.7l3.181 3.182m0-4.991v4.99" /> </svg> `;
const IconBars4 = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M3.75 5.25h16.5m-16.5 4.5h16.5m-16.5 4.5h16.5m-16.5 4.5h16.5" /> </svg>`;
const IconBars3 = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M3.75 6.75h16.5M3.75 12h16.5m-16.5 5.25h16.5" /> </svg>`;
const IconLogout = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M12.75 15l3-3m0 0l-3-3m3 3h-7.5M21 12a9 9 0 11-18 0 9 9 0 0118 0z" /> </svg>`;
const IconSave = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M16.5 3.75V16.5L12 14.25 7.5 16.5V3.75m9 0H18A2.25 2.25 0 0120.25 6v12A2.25 2.25 0 0118 20.25H6A2.25 2.25 0 013.75 18V6A2.25 2.25 0 016 3.75h1.5m9 0h-9" /> </svg>`;
const IconEmail = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M21.75 6.75v10.5a2.25 2.25 0 01-2.25 2.25h-15a2.25 2.25 0 01-2.25-2.25V6.75m19.5 0A2.25 2.25 0 0019.5 4.5h-15a2.25 2.25 0 00-2.25 2.25m19.5 0v.243a2.25 2.25 0 01-1.07 1.916l-7.5 4.615a2.25 2.25 0 01-2.36 0L3.32 8.91a2.25 2.25 0 01-1.07-1.916V6.75" /> </svg>`;
const IconExpand = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M3.75 3.75v4.5m0-4.5h4.5m-4.5 0L9 9M3.75 20.25v-4.5m0 4.5h4.5m-4.5 0L9 15M20.25 3.75h-4.5m4.5 0v4.5m0-4.5L15 9m5.25 11.25h-4.5m4.5 0v-4.5m0 4.5L15 15" /> </svg>`;
const IconShrink = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M9 9V4.5M9 9H4.5M9 9L3.75 3.75M9 15v4.5M9 15H4.5M9 15l-5.25 5.25M15 9h4.5M15 9V4.5M15 9l5.25-5.25M15 15h4.5M15 15v4.5m0-4.5l5.25 5.25" /> </svg>`;
const IconOk = props => html`<svg class=${props.class} fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor" aria-hidden="true"> <path stroke-linecap="round" stroke-linejoin="round" d="M9 12.75L11.25 15 15 9.75M21 12a9 9 0 11-18 0 9 9 0 0118 0z" /> </svg>`;
const IconFailed = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M9.75 9.75l4.5 4.5m0-4.5l-4.5 4.5M21 12a9 9 0 11-18 0 9 9 0 0118 0z" /> </svg>`;
const IconUpload = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M3 16.5v2.25A2.25 2.25 0 005.25 21h13.5A2.25 2.25 0 0021 18.75V16.5m-13.5-9L12 3m0 0l4.5 4.5M12 3v13.5" /> </svg> `;
const IconDownload = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M3 16.5v2.25A2.25 2.25 0 005.25 21h13.5A2.25 2.25 0 0021 18.75V16.5M16.5 12L12 16.5m0 0L7.5 12m4.5 4.5V3" /> </svg> `;
const IconBolt = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M3.75 13.5l10.5-11.25L12 10.5h8.25L9.75 21.75 12 13.5H3.75z" /> </svg>`;
const IconHome = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M2.25 12l8.954-8.955c.44-.439 1.152-.439 1.591 0L21.75 12M4.5 9.75v10.125c0 .621.504 1.125 1.125 1.125H9.75v-4.875c0-.621.504-1.125 1.125-1.125h2.25c.621 0 1.125.504 1.125 1.125V21h4.125c.621 0 1.125-.504 1.125-1.125V9.75M8.25 21h8.25" /> </svg> `;
const IconLink = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M13.19 8.688a4.5 4.5 0 011.242 7.244l-4.5 4.5a4.5 4.5 0 01-6.364-6.364l1.757-1.757m13.35-.622l1.757-1.757a4.5 4.5 0 00-6.364-6.364l-4.5 4.5a4.5 4.5 0 001.242 7.244" /> </svg> `;
const IconShield = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M9 12.75L11.25 15 15 9.75m-3-7.036A11.959 11.959 0 013.598 6 11.99 11.99 0 003 9.749c0 5.592 3.824 10.29 9 11.623 5.176-1.332 9-6.03 9-11.622 0-1.31-.21-2.571-.598-3.751h-.152c-3.196 0-6.1-1.248-8.25-3.285z" /> </svg> `;
const IconBarsDown = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M3 4.5h14.25M3 9h9.75M3 13.5h9.75m4.5-4.5v12m0 0l-3.75-3.75M17.25 21L21 17.25" /> </svg> `;
const IconArrowDown = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M12 4.5v15m0 0l6.75-6.75M12 19.5l-6.75-6.75" /> </svg> `;
const IconArrowUp = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M12 19.5v-15m0 0l-6.75 6.75M12 4.5l6.75 6.75" /> </svg>`;
const IconExclamationTriangle = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M12 9v3.75m-9.303 3.376c-.866 1.5.217 3.374 1.948 3.374h14.71c1.73 0 2.813-1.874 1.948-3.374L13.949 3.378c-.866-1.5-3.032-1.5-3.898 0L2.697 16.126zM12 15.75h.007v.008H12v-.008z" /> </svg>`;

const tipColors = {
  green: 'bg-green-100 text-green-900',
  yellow: 'bg-yellow-100 text-yellow-900',
  red: 'bg-red-100 text-red-900',
};

function Button({title, onclick, disabled, cls, icon, ref, colors, hovercolor, disabledcolor}) {
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
<button type="button" class="inline-flex justify-center items-center gap-1 rounded px-2.5 py-1.5 text-sm font-semibold text-white shadow-sm ${colors} ${cls}"
  ref=${ref} onclick=${cb} disabled=${disabled || spin} >
  ${title}
  <${spin ? IconRefresh : icon} class="w-4 ${spin ? 'animate-spin' : ''}" />
<//>`
};

function Toast({ok, text, close}) {
  const closebtn = useRef(null);
  const from = 'translate-y-2 opacity-0 sm:translate-y-0 sm:translate-x-2';
  const to = 'translate-y-0 opacity-100 sm:translate-x-0';
  const [tr, setTr] = useState(from);
  useEffect(function() {
    setTr(to); 
    setTimeout(ev => closebtn && closebtn.current.click && closebtn.current.click(), 1500);
  }, []);
  const onclose = ev => { setTr(from); setTimeout(close, 300); };
  return html`
<div aria-live="assertive" class="z-10 pointer-events-none absolute inset-0 flex items-end px-4 py-6 sm:items-start sm:p-6">
  <div class="flex w-full flex-col items-center space-y-4 sm:items-end">
    <div class="pointer-events-auto w-full max-w-sm overflow-hidden rounded-lg bg-white shadow-lg ring-1 ring-black ring-opacity-5 transform ease-out duration-300 transition ${tr}">
      <div class="p-4">
        <div class="flex items-start">
          <div class="flex-shrink-0">
            <${ok ? IconOk : IconFailed} class="h-6 w-6 ${ok ? 'text-green-400' : 'text-red-400'}" />
          <//>
          <div class="ml-3 w-0 flex-1 pt-0.5">
            <p class="text-sm font-medium text-gray-900">${text}</p>
            <p class="hidden mt-1 text-sm text-gray-500">Anyone with a link can now view this file.</p>
          <//>
          <div class="ml-4 flex flex-shrink-0">
            <button type="button" ref=${closebtn} onclick=${onclose} class="inline-flex rounded-md bg-white text-gray-400 hover:text-gray-500 focus:outline-none">
              <span class="sr-only">Close</span>
              <svg class="h-5 w-5" viewBox="0 0 20 20" fill="currentColor" aria-hidden="true">
                <path d="M6.28 5.22a.75.75 0 00-1.06 1.06L8.94 10l-3.72 3.72a.75.75 0 101.06 1.06L10 11.06l3.72 3.72a.75.75 0 101.06-1.06L11.06 10l3.72-3.72a.75.75 0 00-1.06-1.06L10 8.94 6.28 5.22z" />
              <//>
            <//>
          <//>
        <//>
      <//>
    <//>
  <//>
<//>`;
};

function Login({login}) {
  const [user, setUser] = useState('');
  const [pass, setPass] = useState('');
  const onsubmit = function(ev) {
    const authhdr = 'Basic ' + btoa(user + ':' + pass);
    const headers = {Authorization: authhdr};
    return fetch('api/login', {headers}).then(login).finally(r => setPass(''));
  };
  return html`
<div class="h-full flex items-center justify-center bg-slate-100">
  <div class="border rounded bg-white w-96 p-5 shadow-sm">
    <div class="my-5 py-2 flex items-center justify-center gap-x-4">
      <${Logo} class="h-12 stroke-cyan-600 stroke-1" />
      <h1 class="font-bold text-xl">WiFi Router Login<//>
    <//>
    <div class="my-3">
      <label class="block text-sm mb-1 dark:text-white">Username</label>
      <input type="text" autocomplete="current-user" required
        class="font-normal bg-white rounded border border-gray-300 w-full 
        flex-1 py-0.5 px-2 text-gray-900 placeholder:text-gray-400
        focus:outline-none sm:text-sm sm:leading-6 disabled:cursor-not-allowed
        disabled:bg-gray-100 disabled:text-gray-500"
        oninput=${ev => setUser(ev.target.value)} value=${user}  />
    <//>
    <div class="my-3">
      <label class="block text-sm mb-1 dark:text-white">Password</label>
      <input type="password" autocomplete="current-password" required
        class="font-normal bg-white rounded border border-gray-300 w-full flex-1 py-0.5 px-2 text-gray-900 placeholder:text-gray-400 focus:outline-none sm:text-sm sm:leading-6 disabled:cursor-not-allowed disabled:bg-gray-100 disabled:text-gray-500"
        oninput=${ev => setPass(ev.target.value)}
        value=${pass} onchange=${onsubmit} />
    <//>
    <div class="mt-7">
      <${Button} title="Sign In" icon=${IconLogout} onclick=${onsubmit} cls="flex w-full justify-center" />
    <//>
    <div class="mt-5 text-slate-400 text-xs">
      To login, use: admin/admin, user1/user1, user2/user2
    <//>
  <//>
<//>`;
};

function Header({logout, user, setShowSidebar, showSidebar}) {
  return html`
<div class="bg-white sticky top-0 z-[48] xw-full border-b py-2 ${showSidebar && 'pl-72'} transition-all duration-300 transform">
  <div class="px-2 w-full py-0 my-0 flex items-center">
    <button type="button" onclick=${ev => setShowSidebar(v => !v)} class="text-slate-400">
      <${IconBars3} class="h-6" />
    <//>
    <div class="flex flex-1 gap-x-4 self-stretch lg:gap-x-6">
      <div class="relative flex flex-1"><//>
      <div class="flex items-center gap-x-4 lg:gap-x-6">
        <span class="text-sm text-slate-400">logged in as: ${user}<//>
        <div class="hidden lg:block lg:h-4 lg:w-px lg:bg-gray-200" aria-hidden="true"><//>
        <${Button} title="Logout" icon=${IconLogout} onclick=${logout} />
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
      <${NavLink} title="Dashboard" icon=${IconHome} href="/" url=${url} />
      <${NavLink} title="Connected Devices" icon=${IconLink} href="/devices" url=${url} />
      <${NavLink} title="Firewall" icon=${IconShield} href="/firewall" url=${url} />
      <${NavLink} title="DHCP" icon=${IconBarsDown} href="/dhcp" url=${url} />
      <${NavLink} title="Administration" icon=${IconSettings} href="/admin" url=${url} />
    <//>
  <//>
<//>`;
};

function Colored({icon, text, colors}) {
  return html`
<span class="inline-flex items-center gap-1.5 py-0.5 px-2 rounded-full ${colors || 'bg-slate-100 text-slate-900'}">
  ${icon && html`<${icon} class="w-5 h-5" />`}
  <span class="inline-block text-xs font-medium">${text}<//>
<//>`;
};

function Stat({title, text, tipText, tipIcon, tipColors}) {
  return html`
<div class="flex flex-col bg-white border shadow-sm rounded-xl dark:bg-slate-900 dark:border-gray-800">
  <div class="p-4 md:p-5">
    <div class="flex items-center gap-x-2">
      <p class="text-xs uppercase tracking-wide text-gray-500"> ${title} </p>
    <//>
    <div class="mt-1 flex items-center gap-x-2">
      <h3 class="text-xl sm:text-2xl font-medium text-gray-800 dark:text-gray-200">
        ${text}
      <//>
      <span class="flex items-center">
        <${Colored} text=${tipText} icon=${tipIcon} colors=${tipColors} />
      <//>
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
      <${Stat} title="Download Speed" text="${stats.download_speed[0]} Mbps" tipText="slow" tipIcon=${IconAlert} tipColors=${tipColors.red} />
      <${Stat} title="Upload Speed" text="${stats.upload_speed[0]} Mbps" tipText="good" tipIcon=${IconOk} tipColors=${tipColors.green} />
      <${Stat} title="Daily Downloaded" text="${stats.downloaded} Mb" tipText="-2.2%" tipIcon=${IconArrowDown} />
      <${Stat} title="Daily Uploaded" text="${stats.uploaded} Mb" tipText="+12.7%" tipIcon=${IconArrowUp} />
    <//>
  <//>
  <div class="grid grid-cols-1 lg:grid-cols-2 gap-2">
    <${SpeedChart} stats=${stats} />
    <${Events} />
    <${ModemStatus} />
  <//>
<//>`;
};

function TextValue({value, setfn, disabled, placeholder, type, addonRight, addonLeft, attr}) {
  const f = type == 'number' ? x => setfn(parseInt(x)) : setfn;
  return html`
<div class="flex w-full items-center rounded border shadow-sm">
  ${ addonLeft && html`<span class="inline-flex font-normal truncate py-1 border-r bg-slate-100 items-center border-gray-300 px-2 text-gray-500 text-xs">${addonLeft}</>` }
  <input type=${type || 'text'} disabled=${disabled} 
    oninput=${ev => f(ev.target.value)} ...${attr}
    class="font-normal text-sm rounded w-full flex-1 py-0.5 px-2 text-gray-700 placeholder:text-gray-400 focus:outline-none disabled:cursor-not-allowed disabled:bg-gray-100 disabled:text-gray-500" placeholder=${placeholder} value=${value} />
  ${ addonRight && html`<span class="inline-flex font-normal truncate py-1 border-l bg-slate-100 items-center border-gray-300 px-2 text-gray-500 text-xs">${addonRight}</>` }
<//>`;
};

function SelectValue({value, setfn, options}) {
  const toInt = x => x == parseInt(x) ? parseInt(x) : x;
  const onchange = ev => setfn(toInt(ev.target.value));
  return html`
<select onchange=${onchange} class="w-full rounded font-normal border py-0.5 px-1 text-gray-600 focus:outline-none text-sm">
  ${options.map(v => html`<option value=${v[0]} selected=${v[0] == value}>${v[1]}<//>`) }
<//>`;
};

function SwitchValue({value, setfn}) {
  const onclick = ev => setfn(!value);
  const bg = !!value ? 'bg-blue-600' : 'bg-gray-200';
  const tr = !!value ? 'translate-x-5' : 'translate-x-0';
  return html`
<button type="button" onclick=${onclick} class="${bg} inline-flex h-6 w-11 flex-shrink-0 cursor-pointer rounded-full border-2 border-transparent transition-colors duration-200 ease-in-out focus:outline-none focus:ring-0 ring-0" role="switch" aria-checked=${!!value}>
  <span aria-hidden="true" class="${tr} pointer-events-none inline-block h-5 w-5 transform rounded-full bg-white shadow ring-0 focus:ring-0 transition duration-200 ease-in-out"></span>
</button>`;
};

function Setting(props) {
  return html`
<div class=${props.cls || 'grid grid-cols-2 gap-2 my-1'}>
  <label class="flex items-center text-sm text-gray-700 mr-2 font-medium">${props.title}<//>
  <div class="flex items-center">
    ${props.type == 'switch' ? h(SwitchValue, props) : 
      props.type == 'select' ? h(SelectValue, props) :
      h(TextValue, props) }
  <//>
<//>`;
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
      ${saveResult && html`<${Toast} ok=${saveResult.status}
        text=${saveResult.message} close=${() => setSaveResult(null)} />`}

      <${Setting} title="Enabled" value=${dhcp.enabled} setfn=${mksetfn('enabled')} type="switch" />
      <${Setting} title="Start Address" value=${dhcp.address_begin} setfn=${mksetfn('address_begin')} type="number" addonLeft="192.168.0." disabled=${!dhcp.enabled} />
      <${Setting} title="End Address" value=${dhcp.address_end} setfn=${mksetfn('address_end')} type="number" addonLeft="192.168.0." disabled=${!dhcp.enabled} />
      <${Setting} title="Lease Time" value=${dhcp.lease_time_sec} setfn=${mksetfn('lease_time_sec')} type="number" addonRight="seconds" disabled=${!dhcp.enabled} />
      <div class="mb-1 mt-3 flex place-content-end"><${Button} icon=${IconSave} onclick=${onsave} title="Save Settings" /><//>
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
  if (!user) return html`<${Login} login=${login} />`; // If not logged in, show login screen

  return html`
<div class="min-h-screen bg-slate-100">
  <${Sidebar} url=${url} show=${showSidebar} />
  <${Header} logout=${logout} user=${user} showSidebar=${showSidebar} setShowSidebar=${setShowSidebar} />
  <div class="${showSidebar && 'pl-72'} transition-all duration-300 transform">
    <${Router} onChange=${ev => setUrl(ev.url)} history=${History.createHashHistory()} >
      <${Main} default=${true} />
      <${DHCP} path="dhcp" />
    <//>
  <//>
<//>`;
};

window.onload = ev => render(h(App), document.body);
