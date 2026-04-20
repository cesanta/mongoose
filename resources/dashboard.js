// SPDX-FileCopyrightText: 2026 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial
(function (global) {
  "use strict";

  const Rpc = (function() {
    function Rpc(url) {
      let ws, id = 1, oncall, q = [], debug = false;
      const pending = new Map();
      const realsend = s => { ws.send(s); if (debug) console.log('WS ->', s); };
      const fail = e => { for (const p of pending.values()) p[1](e || 'disconnected'); pending.clear(); };
      const send = x => ws && ws.readyState === 1
        ? realsend(JSON.stringify(x))
        : q.push(x);
      const connect = () => {
        if (debug) console.log('WS connect');
        ws = new WebSocket(url);
        ws.onopen = () => { if (debug) console.log('WS open'); while (q.length) realsend(JSON.stringify(q.shift())); };
        ws.onclose = () => { fail(); setTimeout(connect, 1000); };
        ws.onmessage = e => {
          if (debug) console.log('WS <-', e.data);
          let m;
          try {
            m = JSON.parse(e.data);
          } catch(err) {
            console.log('INVALID JSON:', e.data);
            return;
          }

          if (m.id && pending.has(m.id)) {
            const [ok, bad] = pending.get(m.id);
            pending.delete(m.id);
            m.error ? bad(m.error) : ok(m.result);
          } else if (oncall && m.method) {
            Promise.resolve(oncall(m.method, m.params))
              .then(r => m.id && send({ id: m.id, result: r }))
              .catch(err => m.id && send({
                id: m.id,
                error: { code: -32000, message: String(err?.message || err) }
              }));
          }
        };
      };

      connect();
      return {
        debug: arg => debug = arg,
        call: (method, params) => new Promise((ok, bad) => {
          const i = id++;
          pending.set(i, [ok, bad]);
          send({  id: i, method, params });
        }),
        notify: (method, params) => send({ method, params }),
        handle: fn => (oncall = fn, this),
        get ws() { return ws; }
      };
    }

    return Rpc;
  })();

  if (global.Dashboard) return;
  const isMock = !!window.frameElement || (window !== window.top);
  const rpc = isMock ? null : Rpc('api/websocket');
  const settings = {data: {}, edits: {}, debug: true};
  const substitutions = {};
  const userhandlers = {}; // User event handlers

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
    }
    return result;
  }

  function edit(el, key, val) {
    const dv = get(settings.data, key);
    if (typeof(dv) === 'number') {
      const v = +val;
      if (isNaN(v)) {
        el.classList.toggle("error", true);
      } else {
        el.classList.toggle("error", false);
        val = v;
      }
    } else if (typeof(v) === 'boolean') {
      val = !!val;
    }

    set(settings.edits, key, val);
    if (val == dv) {
      del(settings.edits, key);
      el.classList.toggle("edited", false);
    } else {
      el.classList.toggle("edited", true);
    }

    if (el.dataset.autosave) save([key]);
    rescan();
  };

  // Return if any of the keys matching pattern is edited
  function edited(keys) {
    for (const key of Object.keys(settings.edits)) {
      if (keys.includes(key)) return true;
    }
    return false;
  };

  function scan_bind(el, key, context) {
    if (!has(context, key)) { console.error('EVAL', key, context); return; }
    const v = get(context, key)
    // console.log(2, key, v);
    if (['INPUT', 'TEXTAREA', 'SELECT'].includes(el.tagName)) {
      if (el.type == 'checkbox') el.checked = v;
      if (el.type == 'text') el.value = v;
      if (!has(settings.edits, key)) {
        el.classList.toggle('edited', false);
        el.classList.toggle('error', false);
      }
      if (!el.bound) el.addEventListener("input", ev => edit(el, key, el.type == 'checkbox' ? ev.target.checked : ev.target.value)), el.bound = true;
    } else {
      el.innerHTML = v;
    }
  };

  // Save the edits for keys that match pattern
  function save(keys) {
    const changes = {};
    for (const key of keys) {
      if (!has(settings.edits, key)) continue;
      set(changes, key, get(settings.edits, key));
    }
    rpc.call('set', changes).then(r => apply_and_rescan(settings.data, r));
  };

  // Cancel the edits for keys that match pattern
  function cancel(keys) {
    for (const key of Object.keys(settings.edits)) {
      if (keys.includes(key)) delete settings.edits[key];
    }
    rescan();
  };

  function scan_save(el, keys) {
    el.disabled = !edited(keys);
    if (!el.bound) el.addEventListener("click", ev => save(keys)), el.bound = true;
  };

  function scan_cancel(el, keys) {
    if (!el.bound) el.addEventListener("click", ev => cancel(keys)), el.bound = true;
    el.disabled = !edited(keys);
  };

  function createSubstitutions() {
    substitutions.nodes = [], substitutions.attributes = [], substitutions.bound = [];
    document.querySelectorAll("*").forEach(function (el) {
      if (el.tagName === "SCRIPT" || el.tagName === "STYLE") return;
      for (const node of el.childNodes) {
        if (node.nodeType === Node.TEXT_NODE && node.nodeValue.includes('${')) {
          node.originalValue = node.nodeValue;
          substitutions.nodes.push(node);
        }
      }
      for (const attr of el.attributes) {
        if (attr.value.includes('${')) {
          attr.originalValue = attr.value;
          substitutions.attributes.push(attr);
        }
      }
      for (const [key, val] of Object.entries(el.dataset)) {
        if (['bind', 'save', 'cancel'].includes(key)) substitutions.bound.push(el);
      }
    });
    // console.log(substitutions);
  };

  function substituteExpressions(text, context) {
    const expected = { string: 1, number: 1, boolean: 1 };
    const f = (orig, expr) => {
      const res = safeEval(expr, context);
      return (typeof res) in expected ? res : orig;
    };
    return typeof text == "string" ? text.replace(/\${(.+?)}/g, f) : text;
  }

  function realrescan() {
    const context = JSON.parse(JSON.stringify(settings.data));
    apply(context, settings.edits);
    if (!substitutions.nodes) createSubstitutions();
    substitutions.attributes.forEach(attr => {
      const v = substituteExpressions(attr.originalValue, context);
      if (v !== attr.value) attr.value = v;
    });
    substitutions.nodes.forEach(node => {
      const v = substituteExpressions(node.originalValue, context);
      if (v !== node.nodeValue) node.nodeValue = v;
    });
    substitutions.bound.forEach(el => {
      for (const [key, val] of Object.entries(el.dataset)) {
        if (key == 'bind') scan_bind(el, val, context);
        if (key == 'save') scan_save(el, val.split(/\s*,\s*/));
        if (key == 'cancel') scan_cancel(el, val.split(/\s*,\s*/));
      }
    });
  };

  function rescan() {
    if (document.readyState === "loading") {
      document.addEventListener("DOMContentLoaded", realrescan, { once: true });
    } else {
      realrescan();
    }
  };

  const delay = (ms, value) => new Promise(r => setTimeout(() => r(value), ms));

  function call(name, args) {
    if (isMock) {
      return delay(750, true).then(val => console.log('response', name));
    } else {
      return rpc.call(name, args).catch(err => console.log('CALL FAILED', name, args, err));
    }
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
    rescan();
  };

  function on(name, fn) {
    userhandlers[name] = fn;
  };

  function init(conf) {
    apply_and_rescan(settings, conf);
    if (rpc) {
      rpc.debug(settings.debug);
      rpc.handle((method, args) => {
        if (method == 'change') apply_and_rescan(settings.data, args);
        if (method != 'change') console.log('UNKNOWN REQUEST', method, args);
        if (userhandlers[method]) userhandlers[method](args);
      });

      // After init, fetch all device data
      rpc.call('get').then(r => apply_and_rescan(settings.data, r));
    }
  };

  global.Dashboard = { init, call, on};
})(window);
