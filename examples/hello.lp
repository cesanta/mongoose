<? mg.write('HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n') ?>
<html><body>
  <span>Today is:</span> <? mg.write(os.date("%A")) ?>
</body></html>
