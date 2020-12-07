const {h, render, Component} = preact;
const html = htm.bind(h);

class Login extends Component {
  state = {user: '', pass: ''};
  onclick = (app) => {
    const authhdr = 'Basic ' + btoa(this.state.user + ':' + this.state.pass);
    const headers = {Authorization: authhdr};
    return axios.get('/api/login', {headers})
        .then(res => app.setUser(res.data.token))
        .catch(err => alert('Login failed'));
  };
  onpassinput = (ev) => this.setState({pass: ev.target.value});
  onuserinput = (ev) => this.setState({user: ev.target.value});
  render({app}, {user, pass, signup}) {
    return html`
      <div class='mx-auto bg-light rounded border my-5' style='max-width: 480px;'>
        <div class='form p-5 rounded form-sm'>
          <h4 class="text-muted mb-4">Login </h4>
          <input type='email' placeholder='Email' class='my-2 form-control'
            oninput=${this.onuserinput} value=${user} />
          <input type="password" placeholder="Password" class="my-2 form-control"
            oninput=${this.onpassinput} value=${pass}
            onchange=${ev => this.onclick(app)} />
          <div class="mb-4">
            <button class="btn btn-info btn-block" 
    disabled=${!user || !pass} onclick="${ev => this.onclick(app)}"
            > Login </button>
          </div>
          <div class="text-muted small">
            Valid logins: admin:admin, user1:pass1, user2:pass2
          </div>
        </div>
      </div>
    `
  }
};

class Dropdown extends Component {
  state = {show: false};
  render(props, state) {
    const onclick = x => this.setState({show: x});
    const show = state.show ? 'show' : '';
    return html`
      <div class="dropdown autoexpand ${props.cls}">
        <div type="buttonx" onclick=${() => onclick(!state.show)}
          class="dropdown-toggle my-0 ${props.bcls}"
          data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">
          ${props.title}
        </div>
        <div onclick=${() => onclick(false)} style=${props.dstyle}
          class="dropdown-menu ${props.dcls} ${show}">
          ${props.children}
        </div>
      </div>`;
  }
};

const NavLink = ({href, title, url}) => html`<a class="nav-item nav-link
    ${url == href ? 'active' : ''}"
    target=${href.match(/^http/) ? '_blank' : ''}
    href="${href.match(/^http/) ? '' : '#'}${href}">${title}</a>`;

class Header extends Component {
  state = {expanded: false};
  ontoggle = () => this.setState({expanded: !this.state.expanded});
  render(props, state) {
    const u = props.app.state.user || {};
    return html`
      <nav class="navbar navbar-expand-md navbar-dark bg-dark">
        <a class="navbar-brand" href="#">
          <img src="images/logo.png" width="26" height="26" alt="" class="mr-2" />
          MyProduct
        </a>
          <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarNav" aria-controls="navbarNav" aria-expanded="false" aria-label="Toggle navigation" 
            onclick=${() => this.ontoggle()} >
          <span class="navbar-toggler-icon"></span>
        </button>
        <div class="navbar-collapse ${state.expanded ? '' : 'collapse'}"
            id="navbarNav">
          <div class="nav navbar-nav mr-auto">
            <${NavLink} href="/" title="Dashboard" url=${props.url} />
            <${NavLink} href="/logs" title="Logs" url=${props.url} />
          </div>
        </div>
        <form class="form-inline">
          <${Dropdown} title="${u.user}" cls="mr-2" 
          bcls="btn btn-sm btn-outline-light pointer" dcls="m-0 dropdown-menu-right">
            <div onclick=${() => props.app.setUser('')}
              class="dropdown-item small pointer text-center">logout</div>
          </${Dropdown}>
          <img src="images/user.png" class="rounded-circle nav-item mr-2" width="30" />
        </form>
      </nav>
    `
  }
};

const Info = () => html`<div class="alert alert-secondary my-3">
  This dashboard shows values kept in server memory. Many clients
  can open this page. As soon as any client changes any value,
  all clients update automatically.
  The JS code that watches state changes, reconnects on network failures.
  That means if server restarts, dashboard on all connected clients
  refresh automatically.
  You can use <code>curl</code> command-line utility:
  <br/><code>curl localhost:8000/api/config/get</code>
  <br/><code>curl localhost:8000/api/config/watch</code>
  <br/><code>curl localhost:8000/api/config/set -d '{"a":123}'</code>
  <br/><span class="badge badge-danger">NOTE:</span> administrators
  can change settings values, whilst users cannot.
</div>`;

class DashVideo extends Component {
  render(props, state) {
    const onclick = ev => axios.post(
        '/api/config/set', {value1: +state.value1, value2: state.value2});
    // alert(JSON.stringify(state));
    return html`<div id="form">
      <div><b>Change values</b></div>
      <table>
        <tr><td>Value 1 (number):</td><td><input value=${state.value1}
          onInput=${linkState(this, 'value1')} /></td></tr>
        <tr><td>Value 2 (string):</td><td><input value=${state.value2}
            onInput=${linkState(this, 'value2')}/></td></tr>
        <tr><td></td>
          <td><button onClick=${onclick}>Save values</button></td></tr>
      </table>
    </div>`
  }
};

class DashForm extends Component {
  render(props, state) {
    const onclick = ev => axios.post(
        '/api/config/set', {value1: +state.value1, value2: state.value2});
    // alert(JSON.stringify(state));
    return html`<div id="form">
      <div><b>Change values</b></div>
      <table>
        <tr><td>Value 1 (number):</td><td><input class="form-control form-control-sm"
          value=${state.value1}
          onInput=${linkState(this, 'value1')} /></td></tr>
        <tr><td>Value 2 (string):</td><td><input class="form-control form-control-sm"
            value=${state.value2}
            onInput=${linkState(this, 'value2')}/></td></tr>
        <tr><td></td>
          <td><button class="btn btn-primary btn-block"
            onClick=${onclick}>Save values</button></td></tr>
      </table>
    </div>`
  }
};

class DashSettings extends Component {
  state = {value1: null, value2: null};
  componentDidMount() {
    axios.get('/api/config/get')
        .then(r => this.setState(r.data))
        .catch(e => console.log(e));
    var self = this;
    var f = function(reader) {
      return reader.read().then(function(result) {
        var data = String.fromCharCode.apply(null, result.value);
        self.setState(JSON.parse(data));
        // console.log(JSON.parse(data));
        if (!result.done) return f(reader);
      });
    };
    fetch('/api/config/watch')
        .then(r => r.body.getReader())
        .then(f)
        .catch(e => setTimeout(x => self.componentDidMount(), 1000));
  }
  render(props, state) {
    return html
    `<div id="dashboard" style="display: flex; justify-content: flex-evenly;">
        <div style="width:50%;text-align:center;">Value 1 (number):
          <div style="font-size:140%;"><b>${state.value1}</b></div></div>
        <div style="width:50%;text-align:center;">Value 2 (string):
          <div style="font-size:140%;"><b>${state.value2}</b></div></div>
    </div>`
  }
};

class AdminDashboard extends Component {
  render(props, state) {
    return html`<div class="container-fluid">
      <${Info} />
      <div class="card-deck">
        <div class="card">
          <div class="card-header">Settings</div>
          <div class="card-body"><${DashSettings} /></div>
        </div>
        <div class="card">
          <div class="card-header">Video stream</div>
          <div class="card-body"><img src="/api/video1" class="mw-100" /></div>
        </div>
        <div class="card">
          <div class="card-header">Change settings</div>
          <div class="card-body"><${DashForm} /></div>
        </div>
      </div>
    </div>`
  }
}

class UserDashboard extends Component {
  render(props, state) {
    return html`<div class="container-fluid">
      <${Info} />
      <div class="card-deck">
        <div class="card">
          <div class="card-header">Settings</div>
          <div class="card-body"><${DashSettings} /></div>
        </div>
        <div class="card">
          <div class="card-header">Video stream</div>
          <div class="card-body"><img src="/api/video1" class="mw-100" /></div>
        </div>
      </div>
    </div>`
  }
}

class Logs extends Component {
  state = {live: '', static: ''};
  componentDidMount() {
    var self = this;
    var f = function(r) {
      return r.read().then(function(result) {
        var data = String.fromCharCode.apply(null, result.value);
        var live = self.state.live + data;
        self.setState({live});   // Append live log
        if (!result.done) f(r);  // Read next chunk
      });
    };
    fetch('/api/log/static')
        .then(r => r.text())
        .then(r => {
          self.setState({static: r});
        })
        .then(r => fetch('/api/log/live'))
        .then(r => r.body.getReader())
        .then(f);
  }
  render(props, state) {
    return html`<div class="container-fluid">
      <div class="alert alert-secondary my-3">
        A div below shows file log.txt, which is produced by the server.
        The server appends a new log message to that file every second,
        and a div below also shows that, implementing a "live log" feature.
        JS code on this page first fetches /api/log/static that returns
        log.txt contents, then fetches /api/log/live that does not return,
        but feeds chunks of data as they arrive (live log).
        <br/><br/>
        You can also use <code>curl</code> command-line utility to see
        live logs:
        <br/><code>curl localhost:8000/api/log/live</code>
      </div>
      <div class="card-deck">
        <div class="card">
          <div class="card-header">Static</div>
          <div class="card-body"><pre class="log">${state.static}</pre></div>
        </div>
        <div class="card">
          <div class="card-header">Live</div>
          <div class="card-body"><pre class="log">${state.live}</pre></div>
        </div>
      </div>
    </div>`
  }
}

class App extends Component {
  state = {user: null, url: '/'};
  setUser(token) {
    const maxAge = token ? 86400 : 0;
    document.cookie = `access_token=${token};path=/;max-age=${maxAge}`;
    // this.setState({token});
    return axios.get('/api/login')
        .then(res => this.setState({user: res.data}))
        .catch(err => this.setState({user: {}}));
  }
  componentDidMount() {
    const getCookie = name => {
      var v = document.cookie.match('(^|;) ?' + name + '=([^;]*)(;|$)');
      return v ? v[2] : '';
    };
    this.setUser(getCookie('access_token'));  // Move from state1 to 2 or 3
  }
  render(props, state) {
    // We have three states:
    // 1. Beginning. We don't know if we have a valid auth token or not
    // 2. We sent an /api/login request, and failed to login
    // 3. We sent an /api/login request, and succeeded to login

    if (!state.user) return '';                          // State 1
    if (!state.user.user) return h(Login, {app: this});  // State 2
    return h(                                            // State 3
        'div', {}, h(Header, {url: state.url, app: this}),
        h(preactRouter.Router, {
          history: History.createHashHistory(),
          onChange: ev => this.setState({url: ev.url}),
        },
          h(state.user.user == 'admin' ? AdminDashboard : UserDashboard,
            {default: true, app: this}),
          h(Logs, {path: 'logs', app: this})));
  }
};

window.onload = () => render(h(App), document.body);
