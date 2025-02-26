'use strict';
import { h, render, useState, useEffect, useRef, html, Router } from  './bundle.js';

// Helper function that returns a promise that resolves after delay
const Delay = (ms, val) => new Promise(resolve => setTimeout(resolve, ms, val));

export const Icons = {
  heart: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M4.318 6.318a4.5 4.5 0 000 6.364L12 20.364l7.682-7.682a4.5 4.5 0 00-6.364-6.364L12 7.636l-1.318-1.318a4.5 4.5 0 00-6.364 0z"></path></svg>`,
  downArrowBox: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M9 8.25H7.5a2.25 2.25 0 00-2.25 2.25v9a2.25 2.25 0 002.25 2.25h9a2.25 2.25 0 002.25-2.25v-9a2.25 2.25 0 00-2.25-2.25H15M9 12l3 3m0 0l3-3m-3 3V2.25" /> </svg>`,
  upArrowBox: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M9 8.25H7.5a2.25 2.25 0 00-2.25 2.25v9a2.25 2.25 0 002.25 2.25h9a2.25 2.25 0 002.25-2.25v-9a2.25 2.25 0 00-2.25-2.25H15m0-3l-3-3m0 0l-3 3m3-3V15" /> </svg>`,
  cog: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M9.594 3.94c.09-.542.56-.94 1.11-.94h2.593c.55 0 1.02.398 1.11.94l.213 1.281c.063.374.313.686.645.87.074.04.147.083.22.127.324.196.72.257 1.075.124l1.217-.456a1.125 1.125 0 011.37.49l1.296 2.247a1.125 1.125 0 01-.26 1.431l-1.003.827c-.293.24-.438.613-.431.992a6.759 6.759 0 010 .255c-.007.378.138.75.43.99l1.005.828c.424.35.534.954.26 1.43l-1.298 2.247a1.125 1.125 0 01-1.369.491l-1.217-.456c-.355-.133-.75-.072-1.076.124a6.57 6.57 0 01-.22.128c-.331.183-.581.495-.644.869l-.213 1.28c-.09.543-.56.941-1.11.941h-2.594c-.55 0-1.02-.398-1.11-.94l-.213-1.281c-.062-.374-.312-.686-.644-.87a6.52 6.52 0 01-.22-.127c-.325-.196-.72-.257-1.076-.124l-1.217.456a1.125 1.125 0 01-1.369-.49l-1.297-2.247a1.125 1.125 0 01.26-1.431l1.004-.827c.292-.24.437-.613.43-.992a6.932 6.932 0 010-.255c.007-.378-.138-.75-.43-.99l-1.004-.828a1.125 1.125 0 01-.26-1.43l1.297-2.247a1.125 1.125 0 011.37-.491l1.216.456c.356.133.751.072 1.076-.124.072-.044.146-.087.22-.128.332-.183.582-.495.644-.869l.214-1.281z" /> <path stroke-linecap="round" stroke-linejoin="round" d="M15 12a3 3 0 11-6 0 3 3 0 016 0z" /> </svg>`,
  settingsH: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M10.5 6h9.75M10.5 6a1.5 1.5 0 11-3 0m3 0a1.5 1.5 0 10-3 0M3.75 6H7.5m3 12h9.75m-9.75 0a1.5 1.5 0 01-3 0m3 0a1.5 1.5 0 00-3 0m-3.75 0H7.5m9-6h3.75m-3.75 0a1.5 1.5 0 01-3 0m3 0a1.5 1.5 0 00-3 0m-9.75 0h9.75" /> </svg>`,
  settingsV: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M6 13.5V3.75m0 9.75a1.5 1.5 0 010 3m0-3a1.5 1.5 0 000 3m0 3.75V16.5m12-3V3.75m0 9.75a1.5 1.5 0 010 3m0-3a1.5 1.5 0 000 3m0 3.75V16.5m-6-9V3.75m0 3.75a1.5 1.5 0 010 3m0-3a1.5 1.5 0 000 3m0 9.75V10.5" /> </svg>`,
  scan: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M7.5 3.75H6A2.25 2.25 0 003.75 6v1.5M16.5 3.75H18A2.25 2.25 0 0120.25 6v1.5m0 9V18A2.25 2.25 0 0118 20.25h-1.5m-9 0H6A2.25 2.25 0 013.75 18v-1.5M15 12a3 3 0 11-6 0 3 3 0 016 0z" /> </svg> `,
  desktop: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M9 17.25v1.007a3 3 0 01-.879 2.122L7.5 21h9l-.621-.621A3 3 0 0115 18.257V17.25m6-12V15a2.25 2.25 0 01-2.25 2.25H5.25A2.25 2.25 0 013 15V5.25m18 0A2.25 2.25 0 0018.75 3H5.25A2.25 2.25 0 003 5.25m18 0V12a2.25 2.25 0 01-2.25 2.25H5.25A2.25 2.25 0 013 12V5.25" /> </svg>`,
  alert: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M14.857 17.082a23.848 23.848 0 005.454-1.31A8.967 8.967 0 0118 9.75v-.7V9A6 6 0 006 9v.75a8.967 8.967 0 01-2.312 6.022c1.733.64 3.56 1.085 5.455 1.31m5.714 0a24.255 24.255 0 01-5.714 0m5.714 0a3 3 0 11-5.714 0M3.124 7.5A8.969 8.969 0 015.292 3m13.416 0a8.969 8.969 0 012.168 4.5" /> </svg>`,
  bell: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M14.857 17.082a23.848 23.848 0 005.454-1.31A8.967 8.967 0 0118 9.75v-.7V9A6 6 0 006 9v.75a8.967 8.967 0 01-2.312 6.022c1.733.64 3.56 1.085 5.455 1.31m5.714 0a24.255 24.255 0 01-5.714 0m5.714 0a3 3 0 11-5.714 0M3.124 7.5A8.969 8.969 0 015.292 3m13.416 0a8.969 8.969 0 012.168 4.5" /> </svg>`,
  refresh: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M16.023 9.348h4.992v-.001M2.985 19.644v-4.992m0 0h4.992m-4.993 0l3.181 3.183a8.25 8.25 0 0013.803-3.7M4.031 9.865a8.25 8.25 0 0113.803-3.7l3.181 3.182m0-4.991v4.99" /> </svg> `,
  bars4: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M3.75 5.25h16.5m-16.5 4.5h16.5m-16.5 4.5h16.5m-16.5 4.5h16.5" /> </svg>`,
  bars3: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M3.75 6.75h16.5M3.75 12h16.5m-16.5 5.25h16.5" /> </svg>`,
  logout: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M12.75 15l3-3m0 0l-3-3m3 3h-7.5M21 12a9 9 0 11-18 0 9 9 0 0118 0z" /> </svg>`,
  save: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M16.5 3.75V16.5L12 14.25 7.5 16.5V3.75m9 0H18A2.25 2.25 0 0120.25 6v12A2.25 2.25 0 0118 20.25H6A2.25 2.25 0 013.75 18V6A2.25 2.25 0 016 3.75h1.5m9 0h-9" /> </svg>`,
  email: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M21.75 6.75v10.5a2.25 2.25 0 01-2.25 2.25h-15a2.25 2.25 0 01-2.25-2.25V6.75m19.5 0A2.25 2.25 0 0019.5 4.5h-15a2.25 2.25 0 00-2.25 2.25m19.5 0v.243a2.25 2.25 0 01-1.07 1.916l-7.5 4.615a2.25 2.25 0 01-2.36 0L3.32 8.91a2.25 2.25 0 01-1.07-1.916V6.75" /> </svg>`,
  expand: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M3.75 3.75v4.5m0-4.5h4.5m-4.5 0L9 9M3.75 20.25v-4.5m0 4.5h4.5m-4.5 0L9 15M20.25 3.75h-4.5m4.5 0v4.5m0-4.5L15 9m5.25 11.25h-4.5m4.5 0v-4.5m0 4.5L15 15" /> </svg>`,
  shrink: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M9 9V4.5M9 9H4.5M9 9L3.75 3.75M9 15v4.5M9 15H4.5M9 15l-5.25 5.25M15 9h4.5M15 9V4.5M15 9l5.25-5.25M15 15h4.5M15 15v4.5m0-4.5l5.25 5.25" /> </svg>`,
  ok: props => html`<svg class=${props.class} fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor" aria-hidden="true"> <path stroke-linecap="round" stroke-linejoin="round" d="M9 12.75L11.25 15 15 9.75M21 12a9 9 0 11-18 0 9 9 0 0118 0z" /> </svg>`,
  fail: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M9.75 9.75l4.5 4.5m0-4.5l-4.5 4.5M21 12a9 9 0 11-18 0 9 9 0 0118 0z" /> </svg>`,
  upload: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M3 16.5v2.25A2.25 2.25 0 005.25 21h13.5A2.25 2.25 0 0021 18.75V16.5m-13.5-9L12 3m0 0l4.5 4.5M12 3v13.5" /> </svg> `,
  download: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M3 16.5v2.25A2.25 2.25 0 005.25 21h13.5A2.25 2.25 0 0021 18.75V16.5M16.5 12L12 16.5m0 0L7.5 12m4.5 4.5V3" /> </svg> `,
  bolt: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M3.75 13.5l10.5-11.25L12 10.5h8.25L9.75 21.75 12 13.5H3.75z" /> </svg>`,
  home: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M2.25 12l8.954-8.955c.44-.439 1.152-.439 1.591 0L21.75 12M4.5 9.75v10.125c0 .621.504 1.125 1.125 1.125H9.75v-4.875c0-.621.504-1.125 1.125-1.125h2.25c.621 0 1.125.504 1.125 1.125V21h4.125c.621 0 1.125-.504 1.125-1.125V9.75M8.25 21h8.25" /> </svg> `,
  link: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M13.19 8.688a4.5 4.5 0 011.242 7.244l-4.5 4.5a4.5 4.5 0 01-6.364-6.364l1.757-1.757m13.35-.622l1.757-1.757a4.5 4.5 0 00-6.364-6.364l-4.5 4.5a4.5 4.5 0 001.242 7.244" /> </svg> `,
  shield: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M9 12.75L11.25 15 15 9.75m-3-7.036A11.959 11.959 0 013.598 6 11.99 11.99 0 003 9.749c0 5.592 3.824 10.29 9 11.623 5.176-1.332 9-6.03 9-11.622 0-1.31-.21-2.571-.598-3.751h-.152c-3.196 0-6.1-1.248-8.25-3.285z" /> </svg> `,
  barsdown: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M3 4.5h14.25M3 9h9.75M3 13.5h9.75m4.5-4.5v12m0 0l-3.75-3.75M17.25 21L21 17.25" /> </svg> `,
  arrowdown: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M12 4.5v15m0 0l6.75-6.75M12 19.5l-6.75-6.75" /> </svg> `,
  arrowup: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M12 19.5v-15m0 0l-6.75 6.75M12 4.5l6.75 6.75" /> </svg>`,
  warn: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M12 9v3.75m-9.303 3.376c-.866 1.5.217 3.374 1.948 3.374h14.71c1.73 0 2.813-1.874 1.948-3.374L13.949 3.378c-.866-1.5-3.032-1.5-3.898 0L2.697 16.126zM12 15.75h.007v.008H12v-.008z" /> </svg>`,
  info: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M11.25 11.25l.041-.02a.75.75 0 011.063.852l-.708 2.836a.75.75 0 001.063.853l.041-.021M21 12a9 9 0 11-18 0 9 9 0 0118 0zm-9-3.75h.008v.008H12V8.25z" /> </svg>`,
  exclamationTriangle: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M12 9v3.75m-9.303 3.376c-.866 1.5.217 3.374 1.948 3.374h14.71c1.73 0 2.813-1.874 1.948-3.374L13.949 3.378c-.866-1.5-3.032-1.5-3.898 0L2.697 16.126zM12 15.75h.007v.008H12v-.008z" /> </svg>`,
  thumbUp: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M6.633 10.5c.806 0 1.533-.446 2.031-1.08a9.041 9.041 0 012.861-2.4c.723-.384 1.35-.956 1.653-1.715a4.498 4.498 0 00.322-1.672V3a.75.75 0 01.75-.75A2.25 2.25 0 0116.5 4.5c0 1.152-.26 2.243-.723 3.218-.266.558.107 1.282.725 1.282h3.126c1.026 0 1.945.694 2.054 1.715.045.422.068.85.068 1.285a11.95 11.95 0 01-2.649 7.521c-.388.482-.987.729-1.605.729H13.48c-.483 0-.964-.078-1.423-.23l-3.114-1.04a4.501 4.501 0 00-1.423-.23H5.904M14.25 9h2.25M5.904 18.75c.083.205.173.405.27.602.197.4-.078.898-.523.898h-.908c-.889 0-1.713-.518-1.972-1.368a12 12 0 01-.521-3.507c0-1.553.295-3.036.831-4.398C3.387 10.203 4.167 9.75 5 9.75h1.053c.472 0 .745.556.5.96a8.958 8.958 0 00-1.302 4.665c0 1.194.232 2.333.654 3.375z" /> </svg>`,
  backward: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M21 16.811c0 .864-.933 1.405-1.683.977l-7.108-4.062a1.125 1.125 0 010-1.953l7.108-4.062A1.125 1.125 0 0121 8.688v8.123zM11.25 16.811c0 .864-.933 1.405-1.683.977l-7.108-4.062a1.125 1.125 0 010-1.953L9.567 7.71a1.125 1.125 0 011.683.977v8.123z" /> </svg>`,
  chip: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M8.25 3v1.5M4.5 8.25H3m18 0h-1.5M4.5 12H3m18 0h-1.5m-15 3.75H3m18 0h-1.5M8.25 19.5V21M12 3v1.5m0 15V21m3.75-18v1.5m0 15V21m-9-1.5h10.5a2.25 2.25 0 002.25-2.25V6.75a2.25 2.25 0 00-2.25-2.25H6.75A2.25 2.25 0 004.5 6.75v10.5a2.25 2.25 0 002.25 2.25zm.75-12h9v9h-9v-9z" /> </svg>`,
  camera: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M6.827 6.175A2.31 2.31 0 015.186 7.23c-.38.054-.757.112-1.134.175C2.999 7.58 2.25 8.507 2.25 9.574V18a2.25 2.25 0 002.25 2.25h15A2.25 2.25 0 0021.75 18V9.574c0-1.067-.75-1.994-1.802-2.169a47.865 47.865 0 00-1.134-.175 2.31 2.31 0 01-1.64-1.055l-.822-1.316a2.192 2.192 0 00-1.736-1.039 48.774 48.774 0 00-5.232 0 2.192 2.192 0 00-1.736 1.039l-.821 1.316z" /> <path stroke-linecap="round" stroke-linejoin="round" d="M16.5 12.75a4.5 4.5 0 11-9 0 4.5 4.5 0 019 0zM18.75 10.5h.008v.008h-.008V10.5z" /> </svg>`,
  arrows: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M7.5 21L3 16.5m0 0L7.5 12M3 16.5h13.5m0-13.5L21 7.5m0 0L16.5 12M21 7.5H7.5" /> </svg>`,
  doc: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M19.5 14.25v-2.625a3.375 3.375 0 00-3.375-3.375h-1.5A1.125 1.125 0 0113.5 7.125v-1.5a3.375 3.375 0 00-3.375-3.375H8.25m2.25 0H5.625c-.621 0-1.125.504-1.125 1.125v17.25c0 .621.504 1.125 1.125 1.125h12.75c.621 0 1.125-.504 1.125-1.125V11.25a9 9 0 00-9-9z" /></svg>`,
};

export const tipColors = {
  green: 'bg-green-100 text-green-900 ring-green-300',
  yellow: 'bg-yellow-100 text-yellow-900 ring-yellow-300',
  red: 'bg-red-100 text-red-900 ring-red-300',
};

export function Button({title, onclick, disabled, cls, icon, ref, colors, hovercolor, disabledcolor}) {
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
<button type="button" class="inline-flex justify-center items-center gap-2 rounded px-2.5 py-1.5 text-sm font-semibold text-white shadow-sm ${colors} ${cls}"
  ref=${ref} onclick=${cb} disabled=${disabled || spin} >
  ${title}
  <${spin ? Icons.refresh : icon} class="w-4 ${spin ? 'animate-spin' : ''}" />
<//>`
};

export function Notification({ok, text, close}) {
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
            <${ok ? Icons.ok : Icons.fail} class="h-6 w-6 ${ok ? 'text-green-400' : 'text-red-400'}" />
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

export function Login({loginFn, logoIcon, title, tipText}) {
  const [user, setUser] = useState('');
  const [pass, setPass] = useState('');
  const onsubmit = function(ev) {
    const authhdr = 'Basic ' + btoa(user + ':' + pass);
    const headers = {Authorization: authhdr};
    return fetch('api/login', {headers}).then(loginFn).finally(r => setPass(''));
  };
  return html`
<div class="h-full flex items-center justify-center bg-slate-200">
  <div class="border rounded bg-white w-96 p-5">
    <div class="my-5 py-2 flex items-center justify-center gap-x-4">
      <${logoIcon} class="h-12 stroke-cyan-600 stroke-1" />
      <h1 class="font-bold text-xl">${title || 'Login'}<//>
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
      <${Button} title="Sign In" icon=${Icons.logout} onclick=${onsubmit} cls="flex w-full justify-center" />
    <//>
    <div class="mt-5 text-slate-400 text-xs">${tipText}<//>
  <//>
<//>`;
};

export function Colored({icon, text, colors}) {
  colors ||= 'bg-slate-100 text-slate-900';
  return html`
<span class="inline-flex items-center gap-1.5 py-0.5">
  ${icon && html`<${icon} class="w-5 h-5" />`}
  <span class="inline-block font-medium rounded-md px-2 py-1 text-xs ring-1 ring-inset ${colors}">${text}<//>
<//>`;
};

export function Stat({title, text, tipText, tipIcon, tipColors, colors}) {
  return html`
<div class="flex flex-col bg-white border shadow-sm rounded-xl dark:bg-slate-900 dark:border-gray-800">
  <div class="overflow-auto rounded-lg bg-white px-4 py-2 ">
    <div class="flex items-center gap-x-2">
      <p class="text-sm truncate text-gray-500 font-medium"> ${title} </p>
    <//>
    <div class="mt-1 flex items-center gap-x-2">
      <h3 class="text-xl truncate font-semibold tracking-tight ${colors || 'text-gray-800 dark:text-gray-200'}">
        ${text}
      <//>
      <span class="flex items-center ${tipText || 'hidden'}">
        <${Colored} text=${tipText} icon=${tipIcon} colors=${tipColors} />
      <//>
    <//>
  <//>
<//>`;
};

export function TextValue({value, setfn, disabled, placeholder, type, addonRight, addonLeft, attr, min, max, step, mult}) {
  const [bg, setBg] = useState('bg-white');
  useEffect(() => { if (type == 'number') checkval(+min, +max, +value); }, []);
  step ||= '1', mult ||= 1;
  const checkval = function(min, max, v) {
    setBg('bg-white');
    if (min && v < min) setBg('bg-red-100 border-red-200');
    if (max && v > max) setBg('bg-red-100 border-red-200');
  };
  const m = step.match(/^.+\.(.+)/);
  const digits = m ? m[1].length : 0;
  const onchange = ev => {
    let v = ev.target.value;
    if (type == 'number') {
      checkval(+min, +max, +v);
      v = +(parseFloat(v) / mult).toFixed(digits);
    }
    setfn(v);
  };
  if (type == 'number') value = +(value * mult).toFixed(digits);
  return html`
<div class="flex w-full items-center rounded border shadow-sm ${bg}">
  ${addonLeft && html`<span class="inline-flex font-normal truncate py-1 border-r bg-slate-100 items-center border-gray-300 px-2 text-gray-500 text-xs">${addonLeft}<//>` }
  <input type=${type || 'text'} disabled=${disabled} value=${value}
    step=${step} min=${min} max=${max} 
    onchange=${onchange} ...${attr}
    class="${bg} font-normal text-sm rounded w-full flex-1 py-0.5 px-2 text-gray-700 placeholder:text-gray-400 focus:outline-none disabled:cursor-not-allowed disabled:bg-gray-100 disabled:text-gray-500" placeholder=${placeholder} />
  ${addonRight && html`<span class="inline-flex font-normal truncate py-1 border-l bg-slate-100 items-center border-gray-300 px-2 text-gray-500 text-xs overflow-scroll" style="min-width: 50%;">${addonRight}<//>` }
<//>`;
};

export function SelectValue({value, setfn, options, disabled}) {
  const toInt = x => x == parseInt(x) ? parseInt(x) : x;
  const onchange = ev => setfn(toInt(ev.target.value));
  return html`
<select onchange=${onchange} class="w-full rounded font-normal border py-0.5 px-1 text-gray-600 focus:outline-none text-sm disabled:cursor-not-allowed" disabled=${disabled}>
  ${options.map(v => html`<option value=${v[0]} selected=${v[0] == value}>${v[1]}<//>`) }
<//>`;
};

export function SwitchValue({value, setfn}) {
  const onclick = ev => setfn(!value);
  const bg = !!value ? 'bg-blue-600' : 'bg-gray-200';
  const tr = !!value ? 'translate-x-5' : 'translate-x-0';
  return html`
<button type="button" onclick=${onclick} class="${bg} inline-flex h-6 w-11 flex-shrink-0 cursor-pointer rounded-full border-2 border-transparent transition-colors duration-200 ease-in-out focus:outline-none focus:ring-0 ring-0" role="switch" aria-checked=${!!value}>
  <span aria-hidden="true" class="${tr} pointer-events-none inline-block h-5 w-5 transform rounded-full bg-white shadow ring-0 focus:ring-0 transition duration-200 ease-in-out"></span>
</button>`;
};

export function Setting(props) {
  let input = TextValue;
  if (props.type == 'switch') input = SwitchValue;
  if (props.type == 'select') input = SelectValue;
  return html`
<div class=${props.cls || 'grid grid-cols-2 gap-2 my-1'}>
  <label class="flex items-center text-sm text-gray-700 mr-2 font-medium ${props.title || 'hidden'}">${props.title}<//>
  <div class="flex items-center">${h(input, props)}<//>
<//>`;
};

export function Pagination({ totalItems, itemsPerPage, currentPage, setPageFn, colors }) {
  const totalPages = Math.ceil(totalItems / itemsPerPage);
  const maxPageRange = 2;
  const lessThanSymbol = "<";
  const greaterThanSymbol = ">";
  const whiteSpace = " ";
  const itemcls = 'relative inline-flex items-center px-3 py-1 text-sm focus:z-20 focus-visible:outline focus-visible:outline-2 focus-visible:outline-offset-0 focus-visible:outline-blue-600';
  colors ||= 'bg-blue-600';

  const PageItem = ({ page, isActive }) => (
    html`<a
      onClick=${() => setPageFn(page)}
      class="${itemcls} ${isActive ? `${colors} text-white` : 'cursor-pointer text-gray-700 ring-1 ring-inset ring-gray-300 hover:bg-gray-50'}"
    >
      ${page}
    </a>`
  );

  return html`
    <div class="flex items-center justify-between bg-white px-3 py-2">
      <div class="sm:flex sm:flex-1 sm:items-center sm:justify-between space-x-4 whitespace-nowrap select-none">
          <p class="text-sm text-slate-500 font-medium">
            showing <span class="font-bold text-slate-700">${(currentPage - 1) * itemsPerPage + 1}</span> - <span class="font-medium">${Math.min(currentPage * itemsPerPage, totalItems)}</span> of ${whiteSpace}
            <span class="font-bold text-slate-700">${totalItems}</span> results
          </p>
        <div>
          <nav class="isolate inline-flex -space-x-px rounded-md" aria-label="Pagination">
            <a
              onClick=${() => setPageFn(Math.max(currentPage - 1, 1))}
              class="relative inline-flex px-3 items-center text-gray-400 ring-1 ring-inset ring-gray-300 hover:bg-gray-50 ${currentPage != 1 ? 'cursor-pointer' : ''} focus:z-20 focus:outline-offset-0">
              ${lessThanSymbol}
            </a>

            <${PageItem} page=${1} isActive=${currentPage === 1} />
            ${currentPage > maxPageRange + 2 ? html`<span class="${itemcls} ring-1 ring-inset ring-gray-300 text-slate-300">...</span>` : ''}
            ${Array.from({length: Math.min(totalPages, maxPageRange * 2 + 1)}, (_, i) => Math.max(2, currentPage - maxPageRange) + i).map(page => page > 1 && page < totalPages && html`<${PageItem} page=${page} isActive=${currentPage === page} />`)}
            ${currentPage < totalPages - (maxPageRange + 1) ? html`<span class="${itemcls} ring-1 ring-inset ring-gray-300 text-slate-300">...</span>` : ''}
            ${totalPages > 1 ? html`<${PageItem} page=${totalPages} isActive=${currentPage === totalPages} />` : ''}

            <a
              onClick=${() => setPageFn(Math.min(currentPage + 1, totalPages))}
              class="relative inline-flex px-3 items-center text-gray-400 ring-1 ring-inset ring-gray-300 hover:bg-gray-50 ${currentPage != totalPages ? 'cursor-pointer' : ''} focus:z-20 focus:outline-offset-0">
              ${greaterThanSymbol}
            </a>
          </nav>
        </div>
      </div>
    </div>`;
};

export function UploadFileButton(props) {
  const [upload, setUpload] = useState(null);  // Upload promise
  const [status, setStatus] = useState('');    // Current upload status
  const btn = useRef(null);
  const input = useRef(null);

  // Send a large file chunk by chunk
  const sendFileData = function(url, fileName, fileData, chunkSize) {
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
        var opts = {method: 'POST', body: chunk};
        var fullUrl = url + '?offset=' + offset +
          '&total=' + fileData.length  +
          '&name=' + encodeURIComponent(fileName);
        var ok;
        setStatus('Uploading ' + fileName + ', bytes ' + offset + '..' +
          (offset + chunk.length) + ' of ' + fileData.length);
        fetch(fullUrl, opts)
          .then(function(res) {
            if (res.ok && chunk.length > 0) sendChunk(offset + chunk.length);
            ok = res.ok;
            return res.text();
          })
          .then(function(text) {
            if (!ok) setStatus('Error: ' + text), finish(ok); // Fail
            if (chunk.length > 0) return; // More chunks to send
            setStatus(x => x + '. Done, resetting device...');
            finish(ok); // All chunks sent
          });
      };
      sendChunk(0);
    });
  };

  const onchange = function(ev) {
    if (!ev.target.files[0]) return;
    let r = new FileReader(), f = ev.target.files[0];
    r.readAsArrayBuffer(f);
    r.onload = function() {
      setUpload(sendFileData(props.url, f.name, new Uint8Array(r.result), 2048));
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

  return html`
<div class="inline-flex flex-col ${props.class}">
  <input class="hidden" type="file" ref=${input} onchange=${onchange} accept=${props.accept} />
  <${Button} title=${props.title} icon=${Icons.download} onclick=${onclick} ref=${btn} colors=${props.colors} />
  <div class="pt-2 text-sm text-slate-400 ${status || 'hidden'}">${status}<//>
<//>`;
};
