const {h, render, Component} = preact;
const html = htm.bind(h);

const Info = () => html`<div id="info">
  This dashboard shows values kept in server memory. Many clients
  can open this page. As soon as any client changes any value,
  all clients update automatically.
  <br/><br/>
  The JS code that watches state changes, reconnects on network failures.
  That means if server restarts, dashboard on all connected clients
  refresh automatically.
  <br/><br/>

  You can use <code>curl</code> command-line utility:
  <br/><code>curl localhost:8000/api/config/get</code>
  <br/><code>curl localhost:8000/api/config/watch</code>
  <br/><code>curl localhost:8000/api/config/set -d '{"a":123}'</code>
</div>`;

class Dashboard extends Component {
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

class Form extends Component {
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

const App = () => html`<div id="container">
  <${Info} />
  <${Dashboard} />
  <${Form} />
</div>`;

window.onload = () => render(h(App), document.body);
