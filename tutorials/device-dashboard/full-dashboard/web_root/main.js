document.addEventListener("DOMContentLoaded", () => {
  const tabs = Array.from(document.querySelectorAll(".header-left .nav-link[href^='#']"));
  const panels = Array.from(document.querySelectorAll(".tab-panel"));

  let chart = null;

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

    requestAnimationFrame(() => {
      const rect = el.getBoundingClientRect();
      if (rect.width <= 0 || rect.height <= 0) rect.height = rect.width = 10;

      if (!chart) {
        options.width = rect.width;
        options.height = rect.height;
        chart = new uPlot(options, [], el);
      } else {
        chart.setSize({ width: rect.width, height: rect.height });
      }
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

  const options = {
    width: 100,
    height: 100,
    tzDate: (ts) => uPlot.tzDate(new Date(ts * 1e3), "Europe/London"),
    plugins: [],
    legend: { show: false },
    axes: [
      { values: "{MMM} {DD}", size: 25 },
      { size: 50, xvalues: (u, vals, space) => vals.map(v => (+v / 1000).toFixed(1) + 'k') },
    ],
    scales: {
      x: { time: true },
      y: { range: [0, 100] },
    },
    series: [
      {},
      {stroke: 'rgba(5, 141, 199, 1)', fill: 'rgba(5, 141, 199, 0.1)'},
      {stroke: 'rgba(237, 126, 23, 1)', fill: 'rgba(237, 126, 23, 0.1)'},
    ],
  };

  function updateGraphPoints(args) {
    if (!args.points || !chart) return;
    const data = [
      args.points.map(x => x[0]),
      args.points.map(x => x[1]),
      args.points.map(x => x[2]),
    ];
    chart.setData(data);
  };

  // Intercept change notifications
  Dashboard.on('change', args => updateGraphPoints(args));
  Dashboard.call('get', 'points').then(resp => updateGraphPoints(resp));

});
