'use strict';
import { h, html, render } from './preact.min.js';


const X = function () {

  return html`
  <div class="container">
    <h2 class="section">ABOUT US</h2>
    <div class="row">
      <div class="col-7">
        <p>      
          Cesanta Software Ltd. is headquartered in Dublin, Republic of Ireland.
        </p>
        <p></p>
        Our story roots back to 2004, when Mongoose Web Server Library development started. <br>

        As Mongoose Web Server grew in popularity and matured over the following years, in 2013 Cesanta was established to continue its development and 
        provide support to our valued customers.
        </p>
        <p>We are proud to have among our customers many <i>Fortune 500</i> companies as well as medium and small size businesses.
          Security and quality of our solutions is a paramount for us and the fact that Mongoose Web Server is used by NASA aboard the International Space Station is 
          the best confirmation to it.</p>
          <p>Since 2013, Cesanta has expanded its product portfolio. We develop and distribute embedded software and hardware with focus on connected products and the Internet of Things.</p>

      </div>
    </div>
  </div>
`;
};

const Y = function () {

  return html`
  <div class="container">
    <h3 class="section">Among our products are:</h3>
    <div class="row">
      <div class="col-7">
       <ul>
         <li>
           <a href="https://mongoose.ws/">Mongoose Web Server</a>
           - an embedded web server and networking library
         </li>
         <li>
           <a href="https://vcon.io/">VCON.io</a>
           - Arduino-compatible boards with built-in firmware OTA updates
           and management dashboard
         </li>
         <li>
           <a href="https://mdash.net/">mDash.net</a>
           - an all-in-one IoT Platform
         </li>
         <li>
           <a href="https://mongoose-os.com">Mongoose OS</a>
           - an operating system for low-power microcontrollers
         </li>

         <li>
           <a href="https://github.com/cesanta/mjs">mJS</a>
           - an embedded JavaScript engine for C/C++
         </li>
       </ul>
      </div>
      <div class="col-6">
        <p><b>Our solutions are:</b></p>
        <ul>
          <li>integrated into thousands of commercial products</li>
          <li>deployed to hundreds of millions devices in production environments</li>
        </ul>
      </div>
    </div>
  </div>
`;
};

const App = function (props) {

  return html`
<h1>Basic Embedded Filesystem demo</h1>
<div>
  ${h(X)}
</div>
<div>
  ${h(Y)}
</div>`;
};

window.onload = () => render(h(App), document.body);
