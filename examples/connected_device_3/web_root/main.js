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
});
