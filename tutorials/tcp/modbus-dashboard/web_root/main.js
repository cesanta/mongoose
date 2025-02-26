//  NOTE: API calls must start with 'api/' in order to serve the app at any URI

'use strict';
import { h, render, useState, useEffect, useRef, html, Router } from  './bundle.js';

const Icons = {
  cog: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M9.594 3.94c.09-.542.56-.94 1.11-.94h2.593c.55 0 1.02.398 1.11.94l.213 1.281c.063.374.313.686.645.87.074.04.147.083.22.127.324.196.72.257 1.075.124l1.217-.456a1.125 1.125 0 011.37.49l1.296 2.247a1.125 1.125 0 01-.26 1.431l-1.003.827c-.293.24-.438.613-.431.992a6.759 6.759 0 010 .255c-.007.378.138.75.43.99l1.005.828c.424.35.534.954.26 1.43l-1.298 2.247a1.125 1.125 0 01-1.369.491l-1.217-.456c-.355-.133-.75-.072-1.076.124a6.57 6.57 0 01-.22.128c-.331.183-.581.495-.644.869l-.213 1.28c-.09.543-.56.941-1.11.941h-2.594c-.55 0-1.02-.398-1.11-.94l-.213-1.281c-.062-.374-.312-.686-.644-.87a6.52 6.52 0 01-.22-.127c-.325-.196-.72-.257-1.076-.124l-1.217.456a1.125 1.125 0 01-1.369-.49l-1.297-2.247a1.125 1.125 0 01.26-1.431l1.004-.827c.292-.24.437-.613.43-.992a6.932 6.932 0 010-.255c.007-.378-.138-.75-.43-.99l-1.004-.828a1.125 1.125 0 01-.26-1.43l1.297-2.247a1.125 1.125 0 011.37-.491l1.216.456c.356.133.751.072 1.076-.124.072-.044.146-.087.22-.128.332-.183.582-.495.644-.869l.214-1.281z" /> <path stroke-linecap="round" stroke-linejoin="round" d="M15 12a3 3 0 11-6 0 3 3 0 016 0z" /> </svg>`,
  info: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M11.25 11.25l.041-.02a.75.75 0 011.063.852l-.708 2.836a.75.75 0 001.063.853l.041-.021M21 12a9 9 0 11-18 0 9 9 0 0118 0zm-9-3.75h.008v.008H12V8.25z" /> </svg>`,
  bolt: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M3.75 13.5l10.5-11.25L12 10.5h8.25L9.75 21.75 12 13.5H3.75z" /> </svg>`,
  home: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M2.25 12l8.954-8.955c.44-.439 1.152-.439 1.591 0L21.75 12M4.5 9.75v10.125c0 .621.504 1.125 1.125 1.125H9.75v-4.875c0-.621.504-1.125 1.125-1.125h2.25c.621 0 1.125.504 1.125 1.125V21h4.125c.621 0 1.125-.504 1.125-1.125V9.75M8.25 21h8.25" /> </svg> `,
  save: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M16.5 3.75V16.5L12 14.25 7.5 16.5V3.75m9 0H18A2.25 2.25 0 0120.25 6v12A2.25 2.25 0 0118 20.25H6A2.25 2.25 0 013.75 18V6A2.25 2.25 0 016 3.75h1.5m9 0h-9" /> </svg>`,
  bars3: props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor"> <path stroke-linecap="round" stroke-linejoin="round" d="M3.75 6.75h16.5M3.75 12h16.5m-16.5 5.25h16.5" /> </svg>`,
};

function TextValue({value, setfn, disabled, placeholder, type, addonRight, addonLeft, attr, min, max, step, mult}) {
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

function SelectValue({value, setfn, options, disabled}) {
  const toInt = x => x == parseInt(x) ? parseInt(x) : x;
  const onchange = ev => setfn(toInt(ev.target.value));
  return html`
<select onchange=${onchange} class="w-full rounded font-normal border py-0.5 px-1 text-gray-600 focus:outline-none text-sm disabled:cursor-not-allowed" disabled=${disabled}>
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
  let input = TextValue;
  if (props.type == 'switch') input = SwitchValue;
  if (props.type == 'select') input = SelectValue;
  return html`
<div class=${props.cls || 'grid grid-cols-2 gap-2 my-1'}>
  <label class="flex items-center text-sm text-gray-700 mr-2 font-medium">${props.title}<//>
  <div class="flex items-center">${h(input, props)}<//>
<//>`;
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
<button type="button" class="inline-flex justify-center items-center gap-2 rounded px-2.5 py-1.5 text-sm font-semibold text-white shadow-sm ${colors} ${cls}"
  ref=${ref} onclick=${cb} disabled=${disabled || spin} >
  ${title}
  <${spin ? Icons.refresh : icon} class="w-4 ${spin ? 'animate-spin' : ''}" />
<//>`
};

function Notification({ok, text, close}) {
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

const Logo = props => html`<svg class=${props.class} xmlns="http://www.w3.org/2000/svg" viewBox="0 0 13 13"><defs><style>.ll-cls-1{fill:none;stroke:#000;stroke-miterlimit:10;stroke-width:0.5px;}</style></defs><g id="Layer_2" data-name="Layer 2"><g id="Layer_1-2" data-name="Layer 1"><path class="ll-cls-1" d="M12.62,1.82V8.91A1.58,1.58,0,0,1,11,10.48H4a1.44,1.44,0,0,1-1-.37A.69.69,0,0,1,2.84,10l-.1-.12a.81.81,0,0,1-.15-.48V5.57a.87.87,0,0,1,.86-.86H4.73V7.28a.86.86,0,0,0,.86.85H9.42a.85.85,0,0,0,.85-.85V3.45A.86.86,0,0,0,10.13,3,.76.76,0,0,0,10,2.84a.29.29,0,0,0-.12-.1,1.49,1.49,0,0,0-1-.37H2.39V1.82A1.57,1.57,0,0,1,4,.25H11A1.57,1.57,0,0,1,12.62,1.82Z"/><path class="ll-cls-1" d="M10.48,10.48V11A1.58,1.58,0,0,1,8.9,12.6H1.82A1.57,1.57,0,0,1,.25,11V3.94A1.57,1.57,0,0,1,1.82,2.37H8.9a1.49,1.49,0,0,1,1,.37l.12.1a.76.76,0,0,1,.11.14.86.86,0,0,1,.14.47V7.28a.85.85,0,0,1-.85.85H8.13V5.57a.86.86,0,0,0-.85-.86H3.45a.87.87,0,0,0-.86.86V9.4a.81.81,0,0,0,.15.48l.1.12a.69.69,0,0,0,.13.11,1.44,1.44,0,0,0,1,.37Z"/></g></g></svg>`;
const ModbusLogo = props => html`
<svg version="1.1" class=${props.class} xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" x="0px" y="0px" viewBox="0 0 652 195">
<style type="text/css"> .mb_st0{fill:#F49500;} .mb_st1{fill:#FCC000;} .mb_st2{fill:#03A239;} .mb_st3{fill:#1192CF;} </style>
<g transform="translate(0 -225)">
<path class="mb_st0" d="M189.4,323.1c0-36.5-31-66.1-69.3-66.1c-38.3,0-69.3,29.6-69.3,66.1c0,36.5,31,66.1,69.3,66.1 C158.4,389.1,189.4,359.6,189.4,323.1"/>
<path class="mb_st1" d="M127.3,251.6c0,14.2-11.5,25.8-25.8,25.8c-14.2,0-25.8-11.5-25.8-25.8c0-14.2,11.5-25.8,25.8-25.8 C115.7,225.8,127.3,237.4,127.3,251.6"/>
<path class="mb_st1" d="M198.9,273.5c0,14.2-11.5,25.8-25.8,25.8c-14.2,0-25.8-11.5-25.8-25.8c0-14.2,11.5-25.8,25.8-25.8 C187.4,247.7,198.9,259.3,198.9,273.5"/>
<path class="mb_st1" d="M223.5,341.5c0,14.2-11.5,25.8-25.8,25.8c-14.2,0-25.8-11.5-25.8-25.8c0-14.2,11.5-25.8,25.8-25.8 C212,315.8,223.5,327.3,223.5,341.5"/>
<path class="mb_st1" d="M168.9,391.3c0,14.2-11.5,25.8-25.8,25.8c-14.2,0-25.8-11.5-25.8-25.8c0-14.2,11.5-25.8,25.8-25.8 C157.3,365.5,168.9,377,168.9,391.3"/>
<path class="mb_st1" d="M94.3,373.4c0,14.2-11.5,25.8-25.8,25.8c-14.2,0-25.8-11.5-25.8-25.8c0-14.2,11.5-25.8,25.8-25.8 C82.7,347.6,94.3,359.2,94.3,373.4"/>
<path class="mb_st1" d="M73.5,303.1c0,14.2-11.5,25.8-25.8,25.8c-14.2,0-25.8-11.5-25.8-25.8c0-14.2,11.5-25.8,25.8-25.8 C61.9,277.3,73.5,288.9,73.5,303.1"/>
<polygon class="mb_st2" points="125.3,308 123.4,301.4 119.4,309 105.1,259.6 112.3,263.8 110.5,257.5 100.5,252 94.9,262 96.8,268.3 100.6,260.9 115,310.3 107.6,306 109.5,312.6 119.6,318 "/> <polygon class="mb_st2" points="137.7,320.1 142.1,316 134.1,315.4 167.7,284.3 167.7,292.1 171.9,288.2 171.7,277.3 160.9,276.3 156.7,280.3 164.5,280.8 130.9,312 131,303.9 126.5,308.1 126.8,319 "/>
<polygon class="mb_st2" points="82.1,373.4 86.6,368.9 78.4,368.7 112.1,335.5 112.2,343.5 116.5,339.3 116.2,328.2 105.1,327.7 100.9,331.9 108.8,332.1 75.1,365.4 75,357.2 70.5,361.6 71,372.7 "/>
<polygon class="mb_st2" points="108.7,312.2 102.3,310.2 106.4,317.6 58.1,302.8 65.4,299 59.3,297.1 49.3,302.5 54.6,312.6 60.7,314.5 56.8,307.3 105,322.1 97.5,325.9 103.9,327.9 113.9,322.4 "/>
<polygon class="mb_st2" points="147.9,378.9 145.9,373 142.1,380.2 127.4,336 134.6,339.3 132.7,333.7 122.6,329.3 117.3,338.8 119.2,344.4 122.9,337.5 137.7,381.7 130.3,378.2 132.3,384.1 142.4,388.5 "/>
<polygon class="mb_st2" points="183.6,332.6 177.6,330.8 181.3,338.1 136.5,325.1 143.3,321.1 137.7,319.5 128.3,325.2 133.2,335 138.8,336.7 135.2,329.6 180,342.5 173,346.6 179,348.4 188.3,342.5 "/>
<polygon class="mb_st3" points="129.6,357.5 143.8,286.8 176.2,286.8 190.8,329.3 222.5,286.8 255,286.8 240.6,357.5 220.6,357.5 230.4,307.5 193.3,357.5 177.6,357.5 159.9,307.6 150.1,357.5 "/>
<path class="mb_st3" d="M320.1,310.8c-2.8-2.8-12.6-4-24-4c-17.8,0-26.6,2.9-31.9,8.2c-5.3,5.3-7.9,25.3-7.9,27.5 c0,2.1-0.8,7.8,3.1,11.7c3.9,3.9,13.3,4.2,27.8,4.2c14.5,0,22.5-2.5,27.9-6.2c5.4-3.6,8.6-22.7,9.2-28.6 C324.8,317.6,322.9,313.5,320.1,310.8 M305.9,333.1c-1.5,5.3-2.1,7.5-3.9,8.8c-1.8,1.3-11,2.2-13,2.2c-2,0-11.8-0.2-13.8-2.2 c-2-2,1-16.9,3.9-18.5c2.9-1.6,8.9-2.1,13.2-2c6.6,0.1,11.6,0.3,13,1.7C306.7,324.7,307.4,327.8,305.9,333.1"/>
<path class="mb_st3" d="M389,286.8l-5.5,28.1c-2.6-6.2-7.5-8.1-18-8.1c-17.8,0-20.9,2.9-26.2,8.2s-7.9,25.3-7.9,27.5 c0,2.1-0.8,7.8,3.1,11.7c3.9,3.9,7,4.2,21.1,4.2c13.6,0,18.5-4.6,20.9-7.7l-1.3,6.8h17.8l13.8-70.7H389z M376,342 c-1.8,1.3-11,2.2-13.1,2.2s-11.8-0.2-13.8-2.2c-2-2,1-16.9,3.9-18.5c2.9-1.6,8.9-2.1,13.2-2c6.6,0.1,11.6,0.3,13.1,1.7 c0.9,0.9,1.5,2.6,1.4,5.1l-1.2,5.9C378.3,338.8,377.7,340.8,376,342"/>
<path class="mb_st3" d="M472.3,309.8c-2.2-1.9-7.8-3-18.4-3c-13.3,0-18.8,2.7-23,8.4l5.5-28.3H419L405,357.5h17.7l1.4-7.2 c2.1,4.9,5.3,8.1,20,8.1c14.5,0,18.4-0.7,23-5.4c4.9-4.9,8.7-23.4,9.2-29.4C476.8,317.7,476,312.9,472.3,309.8 M458.4,333.1 c-1.5,5.3-2.1,7.5-3.9,8.8c-1.8,1.3-11,2.2-13,2.2c-2,0-11.8-0.2-13.8-2.2c-2-2,1-16.9,3.9-18.5c2.9-1.6,8.9-2.1,13.2-2 c6.6,0.1,11.6,0.3,13,1.7C459.3,324.7,459.9,327.8,458.4,333.1"/>
<path class="mb_st3" d="M536.2,307.4h-0.7c0,0-4.7,27.5-9,31.8c-4.3,4.3-10.6,4.6-12.8,4.6c-2.1,0-8.2-0.9-10.4-3.1 c-2.7-2.8,3.2-28.7,4.3-33.3h-17.3c0,0-7,31.7-7,36.6c0,3.9,0.4,11.1,11.2,13.6c2.5,0.6,5.5,0.9,9.2,0.9c2.9,0,5.6-0.3,8-0.9 c8.4-2,13.7-6.8,16.6-10.4l-2,10.4h16.5l9.8-50.1h0H536.2z"/>
<path class="mb_st3" d="M605.6,323h16.1c0,0,0.3-3.9,0.3-5.5c0-1.7,0.3-3.9-3.1-7.3c-3.4-3.4-21.8-3.3-24.9-3.3 c-3.1,0-19.8-0.3-24.5,3.6c-4,3.2-7.3,7.4-7.3,15.6c0,8.3,5.5,11.5,16.4,11.5h19.5c2.7,0,6.1,0.8,6.1,3.3c0,2.5-1.9,5.9-8.7,5.9 h-12.9c-3.1,0-8.7-0.5-8.7-4v-1.6h-16.1c0,0-0.8,2.8-0.8,5.2c0,2.4,0.4,5.5,3.1,8.2c2.7,2.7,14,3.9,25.6,3.9 c11.6,0,24.1-0.5,28.3-4.8c4.3-4.3,6.2-12.3,6.2-16.5c0-4.2-1.4-6.2-2.8-7.6c-1.3-1.3-5.3-3.7-23-3.7c-15.8,0-16.3-1.5-16.3-3 c0-1.2,0.2-4.2,12.7-4.2C603.3,318.6,605.6,319.2,605.6,323"/>
</g>
</svg>`;

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
        <span class="text-sm text-slate-400">developed with Mongoose Library<//>
        <div class="hidden lg:block lg:h-4 lg:w-px lg:bg-gray-200" aria-hidden="true"><//>
        <span><${ModbusLogo} class="h-10" /><//>
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
  const [slaveUrl, setSlaveUrl] = useState('tcp://192.168.0.100:502');
  //const [slaveUrl, setSlaveUrl] = useState('tcp://127.0.0.1:10502');
  const [slaveId, setSlaveId] = useState(1);
  const [startReg, setStartReg] = useState(0);
  const [values, setValues] = useState('0');
  const [numRegs, setNumRegs] = useState(1);
  const [func, setFunc] = useState(3);
  const [connectTimeout, setConnectTimeout] = useState(1500);
  const [response, setResponse] = useState('');

  const onexec = ev => (setResponse('') || 1) && fetch('api/modbus/exec' , {
    method: 'post', body: JSON.stringify({
      url: slaveUrl, id: slaveId, reg: startReg,
      values: (values || '0').split(',').map(v => parseInt(v)),
      timeout: connectTimeout, func: func, nregs: numRegs
    }) 
  }).then(r => r.json())
    .then(r => setResponse(JSON.stringify(r, null, 2)));

  const funcs = [
    [3, '3: Read Multiple Holding Registers'],
    [4, '4: Read Multiple Input Registers'],
    // [6, '6: Write Single Holding Register'],
    [16, '16: Write Multiple Holding Registers'],
    [43, '43: Read Device ID'],
  ];

  const setnumregs = function(val) {          // Number of registers changed
    setValues(Array(val).fill(0).join(','));  // Values: zero-filled array
    return setNumRegs(val);                   // And set the number of regs
  };

  return html`
<div class="m-4 grid grid-cols-1 gap-4 md:grid-cols-1">
  <div class="py-1 divide-y border rounded bg-white flex flex-col">
    <div class="font-light uppercase flex items-center text-gray-600 px-4 py-2">
      Modbus Master Console
    <//>
    <div class="py-2 px-4 grid grid-cols-1 gap-4 md:grid-cols-2">

      <div class="flex-1 flex flex-col relative">
        <${Setting} title="Slave URL" value=${slaveUrl} setfn=${val => setSlaveUrl(val)} type="" />
        <${Setting} title="Connect timeout" value=${connectTimeout} setfn=${val => setConnectTimeout(val)} type="number" addonRight="milliseconds" min="50" max="5000"/>
        <${Setting} title="Slave ID" value=${slaveId} setfn=${val => setSlaveId(val)} type="number" min="1" max="253" addonRight="1 .. 253" />
        <${Setting} title="Starting Register" value=${startReg} setfn=${val => setStartReg(val)} type="number" min="0" max="65535" addonRight="0 .. 65535" />
        <${Setting} title="Register Count" value=${numRegs} setfn=${setnumregs} type="number" min="1" max="125" addonRight="1 .. 125" />
        <${Setting} title="Value(s), e.g. 1 or 1,2,3" value=${values} setfn=${val => setValues(val)} addonRight="1 .. 65535" cls=${func == 16 || func == 6 ? '' : 'hidden'} />
        <${Setting} title="Function" value=${func} setfn=${val => setFunc(val)} type="select" options=${funcs} />
        <div class="mb-1 mt-3 flex place-content-end gap-2 ">
          <${Button} icon=${Icons.bolt} onclick=${onexec} title="Execute" />
        <//>
      <//>

      <div class="flex-1 flex flex-col relative">
        <pre class="flex-1 bg-gray-100 p-2 text-sm overflow-auto max-h-64">${response}<//>
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

  ${saveResult && html`<${Notification} ok=${saveResult.status}
    text=${saveResult.message} close=${() => setSaveResult(null)} />`}

  <div class="py-1 divide-y border rounded bg-white flex flex-col">
    <div class="font-light uppercase flex items-center text-gray-600 px-4 py-2">
      MQTT Forwarding
    <//>
    <div class="py-2 px-5 flex-1 flex flex-col relative">
      <${Setting} title="Enable MQTT forwarding" value=${settings.mqtt_enabled} setfn=${mksetfn('mqtt_enabled')} type="switch" />
      <${Setting} title="MQTT Server URL" value=${settings.mqtt_server_url} setfn=${mksetfn('mqtt_server_url')} />
      <${Setting} title="MQTT Topic RX" value=${settings.mqtt_topic_rx} setfn=${mksetfn('mqtt_topic_rx')} />
      <${Setting} title="MQTT Topic TX" value=${settings.mqtt_topic_tx} setfn=${mksetfn('mqtt_topic_tx')} />
      <div class="mb-1 mt-3 flex place-content-end"><${Button} icon=${Icons.save} onclick=${onsave} title="Save Settings" /><//>
    <//>
  <//>

  <div class="bg-white border rounded-md text-ellipsis overflow-auto" role="alert">
    <${DeveloperNote}>
      <div>
        MQTT forwarding sends MQTT message to the TX topic every time
        a Modbus slave was successfully interrogated, in either manual
        or automatic mode.
      <//>
      <div class="mt-2">
        The RX topic can be used to send Modbus commands over MQTT. The
        command should be a JSON message of the following format:
      <//>
      <div class="mt-2">
        <code>{"url": "tcp://IP:PORT", "reg": NUM, "id": NUM, "func": NUM}<//>
      <//>
    <//>
  <//>

  <div class="py-1 divide-y border rounded bg-white flex flex-col">
    <div class="font-light uppercase flex items-center text-gray-600 px-4 py-2">
      Console Log
    <//>
    <div class="py-2 px-5 flex-1 flex flex-col relative">
      <${Setting} title="Log Level" value=${settings.log_level} setfn=${mksetfn('log_level')} type="select" addonLeft="0-3" options=${logOptions} />
      <div class="mb-1 mt-3 flex place-content-end"><${Button} icon=${Icons.save} onclick=${onsave} title="Save Settings" /><//>
    <//>
  <//>

<//>`;
};

const App = function({}) {
  const [url, setUrl] = useState('/');
  const [showSidebar, setShowSidebar] = useState(true);

  return html`
<div class="min-h-screen bg-slate-100">
  <${Sidebar} url=${url} show=${showSidebar} />
  <${Header} showSidebar=${showSidebar} setShowSidebar=${setShowSidebar} />
  <div class="${showSidebar && 'pl-72'} transition-all duration-300 transform">
    <${Router} onChange=${ev => setUrl(ev.url)} history=${History.createHashHistory()} >
      <${Main} default=${true} />
      <${Settings} path="settings" />
    <//>
  <//>
<//>`;
};

window.onload = () => render(h(App), document.body);
