<?
  -- Lua server pages have full control over the output, including HTTP
  -- headers they send to the client. Send HTTP headers:
  print('HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n')
?><html><body>

<p>This is an example Lua server page served by
<a href="http://code.google.com/p/mongoose">Mongoose web server</a>.
Mongoose has Lua, Sqlite, and other functionality built in the binary.
This example page stores the request in the Sqlite database, and shows
all requests done previously.</p>

<pre>
<?
  -- Open database
  local db = sqlite3.open('requests.db')

  -- Setup a trace callback, to show SQL statements we'll be executing.
  -- db:trace(function(data, sql) print('Executing: ' .. sql .. '\n') end, nil)

  -- Create a table if it is not created already
  db:exec([[
    CREATE TABLE IF NOT EXISTS requests (
      id INTEGER PRIMARY KEY AUTOINCREMENT,
      timestamp NOT NULL,
      method NOT NULL,
      uri NOT NULL,
      user_agent
    );
  ]])

  -- Add entry about this request
  local stmt = db:prepare(
    'INSERT INTO requests VALUES(NULL, datetime("now"), ?, ?, ?);');
  stmt:bind_values(request_info.request_method, request_info.uri,
                   request_info.http_headers['User-Agent'])
  stmt:step()
  stmt:finalize()

  -- Show all previous records
  print('Previous requests:\n')
  stmt = db:prepare('SELECT * FROM requests ORDER BY id DESC;')
  while stmt:step() == sqlite3.ROW do
    local v = stmt:get_values()
    print(v[1] .. ' ' .. v[2] .. ' ' .. v[3] .. ' '
          .. v[4] .. ' ' .. v[5] .. '\n')
  end

  -- Close database
  db:close()
?>
</pre></body></html>
