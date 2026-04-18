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
        ws = new WebSocket(url);
        ws.onopen = () => { while (q.length) realsend(JSON.stringify(q.shift())); };
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
  const bound = {};  // data_key => [ el1, el2, ...]
  const userhandlers = {}; // User event handlers

  function safeEval(expr, context) {
    const c = Object.freeze(context);
    const k = Object.keys(c).join(",");
    return Function("x", `"use strict"; const {${k}} = x; return ${expr};`)(c);
  }

  // function scanAttr(el, name, val, context) {
  //   let res;
  //   try {
  //     res = safeEval(val, context);
  //   } catch (err) {
  //     console.error('EVAL', attr.name, attr.value, context);
  //     //errors.push({type: 'eval', expr: attr.value});
  //     return;
  //   }
  //   //console.log('xxxx', el);
  //   if (name == 'html') {
  //     el.innerHTML = res;
  //   } else {
  //     el[name] = res;
  //   }
  // };

  function edit(el, key, val) {
    if (typeof(settings.data[key]) === 'number') {
      const v = +val;
      if (isNaN(v)) {
        el.classList.toggle("error", true);
      } else {
        el.classList.toggle("error", false);
        val = v;
      }
    } else if (typeof(settings.data[key]) === 'boolean') {
      val = !!val;
    }

    settings.edits[key] = val;
    if (settings.edits[key] == settings.data[key]) {
      delete settings.edits[key];
      el.classList.toggle("edited", false);
    } else {
      el.classList.toggle("edited", true);
    }

    if (el.dataset.autosave) save(new RegExp(key));

    rescan();
  };

  // Return if any of the keys matching pattern is edited
  function edited(regex) {
    for (const key of Object.keys(settings.edits)) {
      if (regex.test(key)) return true;
    }
    return false;
  };

  function scan_bind(el, key, context) {
    if (key in settings.data) {
      if (el.tagName == 'INPUT') {
        if (el.type == 'checkbox') el.checked = context[key];
        if (el.type == 'text') el.value = context[key];
        if (!(key in settings.edits)) {
          el.classList.toggle('edited', false);
          el.classList.toggle('error', false);
        }
        if (!el.bound) el.addEventListener("input", ev => edit(el, key, el.type == 'checkbox' ? ev.target.checked : ev.target.value)), el.bound = true;
      }
    } else {
      console.error(`data-bind: ${key} is not in data`);
    }
  };

  // Save the edits for keys that match pattern
  function save(regex) {
    const changes = {};
    for (const key of Object.keys(settings.edits)) {
      if (regex.test(key)) changes[key] = settings.edits[key];
    }
    rpc.call('set', changes).then(r => apply_and_rescan(settings.data, r));
  };

  // Cancel the edits for keys that match pattern
  function cancel(regex) {
    console.log('CANCELLING');
    for (const key of Object.keys(settings.edits)) {
      if (regex.test(key)) delete settings.edits[key];
    }
    rescan();
  };

  function scan_save(el, pattern) {
    try {
      const re = new RegExp(pattern);
      el.disabled = !edited(re);
      if (!el.bound) el.addEventListener("click", ev => save(re)), el.bound = true;
    } catch (err) {
      console.error('Invalid regex', pattern, 'for data-save');
    }
  };

  function scan_cancel(el, pattern) {
    try {
      const re = new RegExp(pattern);
      if (!el.bound) el.addEventListener("click", ev => cancel(re)), el.bound = true;
      el.disabled = !edited(re);
    } catch (err) {
      console.error('Invalid regex', pattern, 'for data-save');
    }
  };

  function realrescan() {
    const context = {...settings.data, ...settings.edits};
    //console.log('rescanning..', context);
    document.querySelectorAll("*").forEach((el) => {
      if (el.tagName === "SCRIPT" || el.tagName === "STYLE") return;
      for (const [key, val] of Object.entries(el.dataset)) {
        if (key == 'bind') scan_bind(el, val, context);
        if (key == 'save') scan_save(el, val);
        if (key == 'cancel') scan_cancel(el, val);
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
      return rpc.call(name, args)
        .then(r => {
          //if (storedConfig?.debug) console.log('CALL', name, args,  '->', r);
          return r;
        })
        .catch(err => console.log('CALL FAILED', name, args, err));
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
    apply(target, patch)
    for (const key of Object.keys(settings.data)) {
      if (settings.data[key] == settings.edits[key]) delete settings.edits[key];
    }
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
