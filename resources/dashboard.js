// SPDX-FileCopyrightText: 2026 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial
(function (global) {
  "use strict";

  const Watch = (function() {
    function Watch(url, onev, onmsg) {
      let ws, reconnect = true;
      const connect = () => {
        ws = new WebSocket(url);
        ws.onopen = () => onev && onev('open');
        ws.onclose = () => { onev && onev('close'); reconnect && setTimeout(connect, 1000); };
        ws.onmessage = e => {
          onev && onev('message', e.data);
          let m;
          try {
            m = JSON.parse(e.data);
          } catch(err) {
            console.log('INVALID JSON:', e.data);
            return;
          }
          onmsg && onmsg(m);
        };
      };

      connect();
      return {
        close: () => { reconnect = false; ws?.close(); },
        get ws() { return ws; }
      };
    }

    return Watch;
  })();

  function fetch_and_ready() {
    if (isMock) {
      return delay(500).then(rescan).then(mark_ready);
    }
    return fetch('api/get')
      .then(r => r.ok ? r.json() : Promise.reject(r.status))
      .then(data => {
        for (const k in data) {
          if (typeof data[k] === 'number') {
            const prev = settings.data[k];
            const arr = prev && typeof prev === 'object' ? prev : {};
            data[k] = {
              ...arr,
              size: data[k],
              start: typeof arr.start === 'number' ? arr.start : 0,
              data: Array.isArray(arr.data) ? arr.data : [],
            };
          }
        }
        return apply_and_rescan(settings.data, data);
      })
      .then(expand_arrays)
      .then(() => {
        ws_connect();
        return mark_ready();
      })
      .catch(err => console.log('GET failed', err));
  }

  function ws_connect() {
    if (isMock || ws) return;
    ws = Watch('api/websocket', function (evname, args) {
      if (evname == 'open') { status.online = true; rescan(); }
      if (evname == 'close') { status.online = false; rescan(); }
      if (settings.debug) console.log('WS', evname, args);
    }, function (m) {
      if (m.method == 'change' && m.params) {
        const rest = {};
        const arrays = [];
        for (const k in m.params) {
          const ma = k.match(/^(.+)\/(\d+)$/);
          if (ma) {
            const arr = settings.data[ma[1]];
            if (arr?.data) { const i = +ma[2] - (arr.start || 0); if (i >= 0 && i < arr.data.length) arr.data[i] = Object.assign(arr.data[i] || {}, m.params[k]); }
          } else if (typeof m.params[k] === 'number' && settings.data[k]?.data !== undefined) {
            const arr = settings.data[k];
            arr.size = m.params[k];
            if (is_expandable(arr)) arrays.push(expand_array(k, arr.start, arr.end));
          } else {
            rest[k] = m.params[k];
          }
        }
        apply(settings.data, rest);
        prune(settings.data, settings.edits);
        Promise.all(arrays)
          .then(rescan)
          .then(() => userhandlers['change']?.(m.params));
      }
      if (m.method == 'logout') ws_disconnect();
    });
  }

  function ws_disconnect() {
    if (!ws) return;
    ws.close();
    ws = null;
    status.online = false;
  }


  function login(params) {
    if (isMock) return fetch_and_ready().then(() => ({}));
    const opts = {};
    if (params) {
      const username = (params?.username ?? '').trim();
      const password = params?.password ?? '';
      const join = username + ':' + password;
      opts.headers = { Authorization: `Basic ${btoa(join)}` };
    }
    return fetch('api/login', opts)
      .then(r => r.ok ? r.json() : r.text().then(t => Promise.reject(t || r.statusText)))
      .then(resp => {
        status.username = resp?.user || '';
        status.userlevel = resp?.level || 0;
        return fetch_and_ready().then(() => resp);
      });
  };

  function logout() {
    if (isMock) { ws_disconnect(); return Promise.resolve({}); }
    return fetch('api/logout')
      .catch(() => true)
      .then(() => {
        status.username = '';
        status.userlevel = 0;
        ws_disconnect();
        return true;
      });
  }

  if (global.Dashboard) return;
  const isMock = location.protocol === 'file:' || !!window.frameElement || (window !== window.top);
  const settings = {data: {}, edits: {}, debug: true};
  const userhandlers = {}; // User event handlers
  const status = { online: false, username: '', userlevel: 0, ready: false };
  let ws = null;
  let ready_started = false;

  function install_ready_gate() {
    document.documentElement.classList.remove('dashboard-ready');
    if (document.getElementById('dashboard-ready-gate')) return;
    const style = document.createElement('style');
    style.id = 'dashboard-ready-gate';
    style.textContent = `html:not(.dashboard-ready) *,html:not(.dashboard-ready) *::before,html:not(.dashboard-ready) *::after{transition:none!important;animation:none!important}`;
    document.head.appendChild(style);
  }

  function mark_ready() {
    if (ready_started) return Promise.resolve();
    ready_started = true;
    return next_frame().then(() => {
      status.ready = true;
      document.documentElement.classList.add('dashboard-ready');
      userhandlers['ready']?.();
    });
  }

  const fromPath = (path, value) => path.split('.').reverse().reduce((acc, k) => ({ [k]: acc }), value);
  const get = (obj, path) => path.split('.').reduce((o, k) => o?.[k], obj);
  const set = (obj, path, value) => apply(obj, fromPath(path, value));
  const has = (obj, path) => path.split('.').every(k => (obj = obj?.[k]) !== undefined);
  const del = (obj, path) => {
    const keys = path.split('.');
    const last = keys.pop();
    const parent = keys.reduce((o, k) => o?.[k], obj);
    return parent && delete parent[last];
  };
  const eq = (a, b) =>
    a === b ||
    (a && b && typeof a === 'object' && typeof b === 'object' &&
     Object.keys(a).length === Object.keys(b).length &&
     Object.keys(a).every(k => eq(a[k], b[k])));
  const isEmpty = obj => obj && typeof obj === 'object' && Object.keys(obj).length === 0;
  function prune(o1, o2, p = '') {
    for (const k in p ? get(o2, p) : o2) {
      const path = p ? p + '.' + k : k;
      if (!has(o1, path)) continue;
      const x1 = get(o1, path), x2 = get(o2, path);
      if (eq(x1, x2)) del(o2, path);
      if (x2 && typeof (x2) === 'object') prune(o1, o2, path);
      if (isEmpty(get(o2, path))) del(o2, path);
    }
  }

  function safeEval(expr, context) {
    const c = Object.freeze(context);
    const k = Object.keys(c).join(",");
    let result;
    try {
      result = Function("x", `"use strict"; const {${k}} = x; return ${expr};`)(c);
    } catch (err) {
      //console.log('EVAL', expr, context,  err);
    }
    return result;
  }

  function edit(el, key, val) {
    const dv = get(settings.data, key);
    if (typeof(dv) === 'number') {
      const v = +val;
      if (isNaN(v) || val.endsWith('.')) {  // Typing number, endingdot
        el.classList.toggle("error", true);
      } else {
        el.classList.toggle("error", false);
        val = v;
      }
    } else if (typeof(dv) === 'boolean') {
      val = !!val;
    }

    set(settings.edits, key, val);
    if (val === dv) {
      del(settings.edits, key);
      el.classList.toggle("edited", false);
    } else {
      el.classList.toggle("edited", true);
    }

    if (el.dataset.autosave) save(key);
    rescan();
  };

  // Return if any of the keys matching pattern is edited
  function edited(key) {
    return has(settings.edits, key);
  };

  function handle_bind(el, key, context) {
    if (!has(context, key)) { console.error('EVAL', key, context); return; }
    const v = get(context, key)
    // console.log(2, key, v);
    if (['INPUT', 'TEXTAREA', 'SELECT'].includes(el.tagName)) {
      if (el.type == 'checkbox') el.checked = v;
      if (el.type == 'text' || el.tagName == 'SELECT') el.value = v;
      if (!has(settings.edits, key)) {
        el.classList.toggle('edited', false);
        el.classList.toggle('error', false);
      }
      if (!el.bound) el.addEventListener("input", ev => edit(el, key, el.type == 'checkbox' ? ev.target.checked : ev.target.value)), el.bound = true;
    } else if (el.tagName == 'BUTTON') {
      if (!el.bound) {
        el.addEventListener("pointerdown", ev => Dashboard.call('set', set({}, key, true)));
        el.addEventListener("pointerup", ev => Dashboard.call('set', set({}, key, false)));
        el.bound = true;
        el.dataset.autosave = 1;
      }
    } else {
     el.innerHTML = v;
    }
  };

  // Save the edits for keys that match pattern
  function save(key) {
    const changes = {};
    set(changes, key, get(settings.edits, key));
    fetch('api/set', {
      method: 'POST',
      headers: {'Content-Type': 'application/json'},
      body: JSON.stringify(changes),
    }).catch(err => console.log('SET failed', key, err));
  };

  // Cancel the edits for keys that match pattern
  function cancel(key) {
    del(settings.edits, key);
    rescan();
  };

  // Set a checkbox's `checked` *property* from a boolean expression. The
  // `checked` content attribute is presence-based - templating its value
  // would not work, so this binds straight to the IDL property instead
  function handle_checked(el, key, context) {
    if (!has(context, key)) { console.error('EVAL', key, context); return; }
    el.checked = !!get(context, key);
  };

  function handle_save(el, key) {
    el.disabled = !edited(key);
    if (!el.bound) el.addEventListener("click", ev => save(key)), el.bound = true;
  };

  function handle_cancel(el, key) {
    if (!el.bound) el.addEventListener("click", ev => cancel(key)), el.bound = true;
    el.disabled = !edited(key);
  };

  function handle_repeat(el, key, context) {
    if (!has(context, key)) { console.error('EVAL', key, context); return; }
    if (!el.orig) el.orig = el.children[0].cloneNode(true);
    const v = get(context, key);
    const it = el.dataset.iterator ?? '__obj';
    const json = v.map(item => JSON.stringify(item));
    const prev = el.itemsJson || [];
    if (json.length === prev.length && el.children.length === json.length) {
      // Same length: re-template each changed row's existing DOM node in
      // place instead of replacing it. Untouched rows are left completely
      // alone, and changed rows keep their live elements too - so bound
      // properties (e.g. a checkbox's `checked`) change on an
      // already-painted node, which is what lets CSS transitions play, and
      // in-progress edits in unrelated fields of that row survive
      json.forEach((j, i) => {
        if (j !== prev[i]) process(el.children[i], { ...context, [it]: v[i] }, true);
      });
    } else {
      const frag = document.createDocumentFragment();
      v.forEach(item => {
        const node = el.orig.cloneNode(true);
        process(node, { ...context, [it]: item }, true);
        frag.appendChild(node);
      });
      el.replaceChildren(frag);
    }
    el.itemsJson = json;
  };

  function upload(el, is_ota) {
    if (el.bound) return;
    el.bound = true;
    el.addEventListener("change", function (ev) {
      const f = ev.target.files[0];
      if (!f) return;
      const reader = new FileReader();
      reader.readAsArrayBuffer(f);
      reader.onload = function () {
        const url = is_ota ? 'api/ota' : `fs/${el.dataset.upload}/${encodeURIComponent(f.name)}`;
        let begin = Date.now(), body = reader.result, ok = false;
        // console.log(2, url, body);
        fetch(url, { method: 'POST', body })
          .then(r => { ok = r.ok; })
          .catch(() => true)
          .finally(() => {
            console.log('UPLOAD finshed in ', Date.now() - begin, 'ms');
          });
        };
      ev.target.value = '';
      ev.preventDefault();
    });
  };

  const handle_upload = el => upload(el, false);
  const handle_ota = el => upload(el, true);

  function substituteExpressions(text, context) {
    const expected = { string: 1, number: 1, boolean: 1 };
    const f = (orig, expr) => {
      const res = safeEval(expr, context);
      return (typeof res) in expected ? res : orig;
    };
    return typeof text == "string" ? text.replace(/\${(.+?)}/g, f) : text;
  }

  function process(root, context, x) {
    const selector = x ? "*" : "*:not([data-repeat] *)";
    const all = root.querySelectorAll(selector);
    const els = root.nodeType === Node.DOCUMENT_NODE ? all : [root, ...all];
    els.forEach(function (el) {
      if (el.tagName === "SCRIPT" || el.tagName === "STYLE") return;
      for (const node of el.childNodes) {
        if (node.nodeType === Node.TEXT_NODE &&
            (node.originalValue || node.nodeValue.includes('${'))) {
          if (!node.originalValue) node.originalValue = node.nodeValue;
          const v = substituteExpressions(node.originalValue, context);
          if (v !== node.nodeValue) node.nodeValue = v;
        }
      }
      for (const attr of el.attributes) {
        const cv = attr.value || '';
        if (!el.originalAttrs) el.originalAttrs = {};
        if (el.originalAttrs[attr.name] || cv.includes('${')) {
          if (!el.originalAttrs[attr.name]) el.originalAttrs[attr.name] = cv;
          const v = substituteExpressions(el.originalAttrs[attr.name], context);
          if (v !== attr.value) attr.value = v;
        }
      }
      const handlers = {
        bind: handle_bind,
        checked: handle_checked,
        save: handle_save,
        cancel: handle_cancel,
        repeat: handle_repeat,
        upload: handle_upload,
        ota: handle_ota,
      };
      for (const [key, val] of Object.entries(el.dataset)) {
        handlers[key]?.(el, val, context);
      }
    });
  };

  function realrescan() {
    // const t0 = performance.now();
    const context = JSON.parse(JSON.stringify(settings.data));
    apply(context, { __status: status });
    apply(context, settings.edits);
    process(document, context);
    // const t1 = performance.now();
    // console.log(`Took2 ${t1 - t0} ms`);
  };

  function rescan() {
    if (document.readyState === "loading") {
      return new Promise(resolve => {
        document.addEventListener("DOMContentLoaded", () => {
          realrescan();
          resolve();
        }, { once: true });
      });
    } else {
      realrescan();
      return Promise.resolve();
    }
  };

  const delay = (ms, value) => new Promise(r => setTimeout(() => r(value), ms));
  const next_frame = () => new Promise(r => requestAnimationFrame(() => r()));
  const is_expandable = arr => arr && Array.isArray(arr.data) &&
    typeof arr.start === 'number' && typeof arr.end === 'number';

  function expand_array(name, start, end) {
    const arr = settings.data[name];
    const page = 100, all = [];
    const stop = Math.min(end, typeof arr?.size === 'number' ? arr.size - 1 : end);
    const done = () => {
      set(settings.data, `${name}.data`, all);
      set(settings.data, `${name}.start`, start);
      return rescan().then(() => all);
    };
    const next = from => {
      if (from > stop) return Promise.resolve(done());
      const to = Math.min(from + page - 1, stop);
      return fetch(`api/get/${name}/${from}/${to}`)
        .then(r => r.ok ? r.json() : Promise.reject(r.statusText))
        .then(data => {
          if (!data || !data.length) return done();
          all.push(...data);
          return next(from + data.length);
        });
    };
    return next(start);
  }

  function expand_arrays() {
    const all = [];
    for (const name in settings.data) {
      const arr = settings.data[name];
      if (is_expandable(arr)) all.push(expand_array(name, arr.start, arr.end));
    }
    return Promise.all(all);
  }

  function call(name, args) {
    if (isMock) {
      return delay(750, true).then(() => console.log('response', name));
    }
    if (name === 'set') {
      return fetch('api/set', {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify(args),
      }).catch(err => console.log('CALL FAILED', name, args, err));
    }
    if (args && typeof args === 'object') {
      const {name: aname, start: s = 0, size: total = 100, end} = args;
      const stop = typeof end === 'number' ? end : s + total - 1;
      return expand_array(aname, s, stop).catch(err => console.log('CALL FAILED', name, args, err));
    }
    const path = typeof args === 'string' && args ? '/' + args : '';
    return fetch('api/get' + path)
      .then(r => r.ok ? r.json() : Promise.reject(r.statusText))
      .catch(err => console.log('CALL FAILED', name, args, err));
  };

  function apply(target, patch) {
    for (const k in patch) {
      const v = patch[k];
      if (v && typeof v === 'object' && !Array.isArray(v)) {
        target[k] = apply(target[k] || {}, v);
      } else {
        target[k] = v;
      }
    }
    return target;
  };

  function apply_and_rescan(target, patch) {
    apply(target, patch);
    prune(settings.data, settings.edits);
    return rescan();
  };

  function on(name, fn) {
    userhandlers[name] = fn;
    if (name === 'ready' && status.ready) setTimeout(fn, 0);
  };

  function init(conf) {
    install_ready_gate();
    apply_and_rescan(settings, conf);
    if (!settings.auth) fetch_and_ready();
  };

  global.Dashboard = { init, call, on, status, login, logout };
})(window);
