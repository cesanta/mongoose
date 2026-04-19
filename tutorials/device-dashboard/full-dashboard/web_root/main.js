document.addEventListener("DOMContentLoaded", () => {
  const tabs = Array.from(document.querySelectorAll(".header-left .nav-link[href^='#']"));
  const panels = Array.from(document.querySelectorAll(".tab-panel"));

  const activateTab = targetId => {
    const targetPanel = document.getElementById(targetId);
    if (!targetPanel) return;

    tabs.forEach(tab => {
      const isActive = tab.getAttribute("href") === `#${targetId}`;
      tab.classList.toggle("nav-link-active", isActive);
      tab.setAttribute("aria-selected", String(isActive));
      tab.setAttribute("tabindex", isActive ? "0" : "-1");
    });

    panels.forEach(panel => {
      const isActive = panel.id === targetId;
      panel.classList.toggle("tab-panel-active", isActive);
      panel.hidden = !isActive;
    });
  };

  tabs.forEach(tab => {
    tab.addEventListener("click", event => {
      event.preventDefault();
      const targetId = tab.getAttribute("href").slice(1);
      activateTab(targetId);
      window.history.replaceState(null, "", `#${targetId}`);
    });
  });

  const initialId = window.location.hash.slice(1) || "overview";
  activateTab(initialId);


  const el = document.getElementById("chart");
  const rect = el.getBoundingClientRect();

  const options = {
    width: rect.width,
    height: rect.height,
    tzDate: (ts) => uPlot.tzDate(new Date(ts * 1e3), "Europe/London"),
    plugins: [],
    legend: { show: false },
    axes: [
      { values: "{MMM} {DD}", size: 25 },
      { size: 50, xvalues: (u, vals, space) => vals.map(v => (+v / 1000).toFixed(1) + 'k') },
    ],
    series: [
      {},
      {stroke: 'rgba(5, 141, 199, 1)', fill: 'rgba(5, 141, 199, 0.1)'},
      {stroke: 'rgba(237, 126, 23, 1)', fill: 'rgba(237, 126, 23, 0.1)'},
    ],
  };

  // Create chart instance
  const chart = new uPlot(options, [], el);

  function updateGraphPoints(args) {
    const data = [
      args.points.map(x => x[0]),
      args.points.map(x => x[1]),
      args.points.map(x => x[2]),
    ];
    //console.log(123, args.points);
    chart.setData(data);
  };

  // Intercept change notifications
  Dashboard.on('change', args => updateGraphPoints(args));
  Dashboard.call('get', 'points').then(resp => updateGraphPoints(resp));

});
