$(document).ready(function() {
  // Start 1-second timer to call RESTful endpoint
  setInterval(function() {
    $.ajax({
      url: '/get_cpu_usage',
      dataType: 'json',
      success: function(json) {
        $('#cpu_usage').text(json.result + '% ');
      }
    });
  }, 1000);

  // Initialize graph
  var $c = $('<div class="graph"/>').appendTo('#graphs');
  var dur = 120;
  var past = Date.now() - dur * 1000;
  var options = {
    lines: {
      fill: true,
      lineWidth: 0,
      fillColor: {colors: [ { opacity: 0 }, { opacity: 1 } ] }
    },
    grid: { borderWidth: 1, borderColor: '#ccc'},
    xaxis: { mode: 'time', ticks: 5 },
    legend: { labelBoxBorderColor: '#fff' },
    colors: [ '#fec', '#396', '#e39', '9e2' ],
    hooks: {
      draw: [function(plot, canvas) {
        canvas.font = '13px sans-serif';
        canvas.fillStyle = '#aaa';
        canvas.fillText('Resource Usage', 35, 25);
      }]
    }
  };
  var plot = $.plot($c, [], options);

  var updateGraph = function(counter) {
    var data = plot.getData();
    var now = Date.now();
    var oldest = now - dur * 1000;
    data[0] = {
      show: false,
      data: [[oldest, null], [now, null]]
    };

    console.log(data);

    // Remove old points
    $.each(data, function(di, d) {
      while (d.data.length > 0 && d.data[0][0] < oldest) {
        d.data.shift();
      }
    });

    // Add new points
    var new_datapoint = [now, counter];
    if (data[1]) {
      data[1].label = 'memory';
      data[1].data.push(new_datapoint);
    } else {
      data[1] = [{ data: [new_datapoint] }];
    }

    // Redraw the graph
    plot.setData(data);
    plot.setupGrid();
    plot.draw();
  };

  // Create Websocket connection. For simplicity, no reconnect logic is here.
  var ws = new WebSocket('ws://' + location.host);
  ws.onmessage = function(ev) {
    updateGraph(ev.data);
  };
});
