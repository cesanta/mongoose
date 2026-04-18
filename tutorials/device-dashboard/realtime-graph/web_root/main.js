document.addEventListener("DOMContentLoaded", function() {

  const el = document.getElementById("graph");
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

  // Intercept change notifications
  Dashboard.on('change', function(args) {
    const data = [
      args.points.map(x => x[0]),
      args.points.map(x => x[1]),
      args.points.map(x => x[2]),
    ];
    //console.log(123, args.points);
    chart.setData(data);
  });
});
