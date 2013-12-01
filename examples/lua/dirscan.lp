HTTP/1.0 200 OK
Content-Type: text/plain

<?
  -- Directory scanning example

  function scandir(dir)
    local t = {}
    for f in io.popen('dir ' .. dir):lines() do table.insert(t, f) end
    return t
  end

  local tab = scandir('C:\\')
  for k,v in pairs(tab) do mg.write(v, '\n') end
?>
