Http = {
  onrcv: function(c, req) {
    if (req.uri === '/data/js') {
      // Send reply. We need to construct everything, headers and body
      mg_send(c, 'HTTP/1.0 200 OK\r\n\r\n');
      mg_send(c, JSON.stringify(req));

      // Let Mongoose know we've handled the connection
      return true;
    }
  }
}
