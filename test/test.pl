#!/usr/bin/env perl
# This script is used to test Mongoose web server
# $Id: test.pl 516 2010-05-03 12:54:37Z valenok $

use IO::Socket;
use File::Path;
use strict;
use warnings;
#use diagnostics;

sub on_windows { $^O =~ /win32/i; }

my $port = 23456;
my $pid = undef;
my $num_requests;
my $dir_separator = on_windows() ? '\\' : '/';
my $copy_cmd = on_windows() ? 'copy' : 'cp';
my $test_dir_uri = "test_dir";
my $root = 'test';
my $test_dir = $root . $dir_separator. $test_dir_uri;
my $config = 'mongoose.conf';
my $exe = '.' . $dir_separator . 'mongoose';
my $embed_exe = '.' . $dir_separator . 'embed';
my $unit_test_exe = '.' . $dir_separator . 'unit_test';
my $exit_code = 0;

my @files_to_delete = ('debug.log', 'access.log', $config, "$root/a/put.txt",
  "$root/a+.txt", "$root/.htpasswd", "$root/binary_file", "$root/a",
  "$root/myperl", $embed_exe, $unit_test_exe);

END {
  unlink @files_to_delete;
  kill_spawned_child();
  File::Path::rmtree($test_dir);
  exit $exit_code;
}

sub fail {
  print "FAILED: @_\n";
  $exit_code = 1;
  exit 1;
}

sub get_num_of_log_entries {
  open FD, "access.log" or return 0;
  my @lines = (<FD>);
  close FD;
  return scalar @lines;
}

# Send the request to the 127.0.0.1:$port and return the reply
sub req {
  my ($request, $inc, $timeout) = @_;
  my $sock = IO::Socket::INET->new(Proto=>"tcp",
    PeerAddr=>'127.0.0.1', PeerPort=>$port);
  fail("Cannot connect: $!") unless $sock;
  $sock->autoflush(1);
  foreach my $byte (split //, $request) {
    last unless print $sock $byte;
    select undef, undef, undef, .001 if length($request) < 256;
  }
  my ($out, $buf) = ('', '');
  eval {
    alarm $timeout if $timeout;
    $out .= $buf while (sysread($sock, $buf, 1024) > 0);
    alarm 0 if $timeout;
  };
  close $sock;

  $num_requests += defined($inc) ? $inc : 1;
  my $num_logs = get_num_of_log_entries();

  unless ($num_requests == $num_logs) {
    fail("Request has not been logged: [$request], output: [$out]");
  }

  return $out;
}

# Send the request. Compare with the expected reply. Fail if no match
sub o {
  my ($request, $expected_reply, $message, $num_logs) = @_;
  print "==> $message ... ";
  my $reply = req($request, $num_logs);
  if ($reply =~ /$expected_reply/s) {
    print "OK\n";
  } else {
#fail("Requested: [$request]\nExpected: [$expected_reply], got: [$reply]");
    fail("Expected: [$expected_reply], got: [$reply]");
  }
}

# Spawn a server listening on specified port
sub spawn {
  my ($cmdline) = @_;
  print 'Executing: ', @_, "\n";
  if (on_windows()) {
    my @args = split /\s+/, $cmdline;
    my $executable = $args[0];
    $executable .= '.exe';
    Win32::Spawn($executable, $cmdline, $pid);
    die "Cannot spawn @_: $!" unless $pid;
  } else {
    unless ($pid = fork()) {
      exec $cmdline;
      die "cannot exec [$cmdline]: $!\n";
    }
  }
  sleep 1;
}

sub write_file {
  open FD, ">$_[0]" or fail "Cannot open $_[0]: $!";
  binmode FD;
  print FD $_[1];
  close FD;
}

sub read_file {
  open FD, $_[0] or fail "Cannot open $_[0]: $!";
  my @lines = <FD>;
  close FD;
  return join '', @lines;
}

sub kill_spawned_child {
  if (defined($pid)) {
    kill(9, $pid);
    waitpid($pid, 0);
  }
}

####################################################### ENTRY POINT

unlink @files_to_delete;
$SIG{PIPE} = 'IGNORE';
$SIG{ALRM} = sub { die "timeout\n" };
#local $| =1;

# Make sure we export only symbols that start with "mg_", and keep local
# symbols static.
if ($^O =~ /darwin|bsd|linux/) {
  my $out = `(cc -c mongoose.c && nm mongoose.o) | grep ' T '`;
  foreach (split /\n/, $out) {
    /T\s+_?mg_.+/ or fail("Exported symbol $_")
  }
}

if (scalar(@ARGV) > 0 and $ARGV[0] eq 'embedded') {
  do_embedded_test();
  exit 0;
}

if (scalar(@ARGV) > 0 and $ARGV[0] eq 'unit') {
  do_unit_test();
  exit 0;
}

# Make sure we load config file if no options are given.
# Command line options override config files settings
write_file($config, "access_log_file access.log\nlistening_ports 12345\n");
spawn("$exe -p $port");
o("GET /test/hello.txt HTTP/1.0\n\n", 'HTTP/1.1 200 OK', 'Loading config file');
unlink $config;
kill_spawned_child();

# Spawn the server on port $port
my $cmd = "$exe ".
  "-listening_ports $port ".
  "-access_log_file access.log ".
  "-error_log_file debug.log ".
  "-cgi_environment CGI_FOO=foo,CGI_BAR=bar,CGI_BAZ=baz " .
  "-extra_mime_types .bar=foo/bar,.tar.gz=blah,.baz=foo " .
  '-put_delete_passwords_file test/passfile ' .
  '-access_control_list -0.0.0.0/0,+127.0.0.1 ' .
  "-document_root $root ".
  "-hide_files_patterns **exploit.pl ".
  "-enable_keep_alive yes ".
  "-url_rewrite_patterns /aiased=/etc/,/ta=$test_dir";
$cmd .= ' -cgi_interpreter perl' if on_windows();
spawn($cmd);

o("GET /hello.txt HTTP/1.1\n\n   GET /hello.txt HTTP/1.0\n\n",
  'HTTP/1.1 200.+keep-alive.+HTTP/1.1 200.+close',
  'Request pipelining', 2);

my $x = 'x=' . 'A' x (200 * 1024);
my $len = length($x);
o("POST /env.cgi HTTP/1.0\r\nContent-Length: $len\r\n\r\n$x",
  '^HTTP/1.1 200 OK', 'Long POST');

# Try to overflow: Send very long request
req('POST ' . '/..' x 100 . 'ABCD' x 3000 . "\n\n", 0); # don't log this one

o("GET /hello.txt HTTP/1.0\n\n", 'HTTP/1.1 200 OK', 'GET regular file');
o("GET /hello.txt HTTP/1.0\nContent-Length: -2147483648\n\n",
  'HTTP/1.1 200 OK', 'Negative content length');
o("GET /hello.txt HTTP/1.0\n\n", 'Content-Length: 17\s',
  'GET regular file Content-Length');
o("GET /%68%65%6c%6c%6f%2e%74%78%74 HTTP/1.0\n\n",
  'HTTP/1.1 200 OK', 'URL-decoding');

# Break CGI reading after 1 second. We must get full output.
# Since CGI script does sleep, we sleep as well and increase request count
# manually.
my $slow_cgi_reply;
print "==> Slow CGI output ... ";
fail('Slow CGI output forward reply=', $slow_cgi_reply) unless
  ($slow_cgi_reply = req("GET /timeout.cgi HTTP/1.0\r\n\r\n", 0, 1)) =~ /Some data/s;
print "OK\n";
sleep 3;
$num_requests++;

# '+' in URI must not be URL-decoded to space
write_file("$root/a+.txt", '');
o("GET /a+.txt HTTP/1.0\n\n", 'HTTP/1.1 200 OK', 'URL-decoding, + in URI');

# Test HTTP version parsing
o("GET / HTTPX/1.0\r\n\r\n", '400 Bad Request', 'Bad HTTP Version', 0);
o("GET / HTTP/x.1\r\n\r\n", '505 HTTP', 'Bad HTTP maj Version');
o("GET / HTTP/1.1z\r\n\r\n", '505 HTTP', 'Bad HTTP min Version');
o("GET / HTTP/02.0\r\n\r\n", '505 HTTP version not supported',
  'HTTP Version >1.1');

# File with leading single dot
o("GET /.leading.dot.txt HTTP/1.0\n\n", 'abc123', 'Leading dot 1');
o("GET /...leading.dot.txt HTTP/1.0\n\n", 'abc123', 'Leading dot 2');
o("GET /../\\\\/.//...leading.dot.txt HTTP/1.0\n\n", 'abc123', 'Leading dot 3')
  if on_windows();
o("GET .. HTTP/1.0\n\n", '400 Bad Request', 'Leading dot 4', 0);

mkdir $test_dir unless -d $test_dir;
o("GET /$test_dir_uri/not_exist HTTP/1.0\n\n",
  'HTTP/1.1 404', 'PATH_INFO loop problem');
o("GET /$test_dir_uri HTTP/1.0\n\n", 'HTTP/1.1 301', 'Directory redirection');
o("GET /$test_dir_uri/ HTTP/1.0\n\n", 'Modified', 'Directory listing');
write_file("$test_dir/index.html", "tralala");
o("GET /$test_dir_uri/ HTTP/1.0\n\n", 'tralala', 'Index substitution');
o("GET / HTTP/1.0\n\n", 'embed.c', 'Directory listing - file name');
o("GET /ta/ HTTP/1.0\n\n", 'Modified', 'Aliases');
o("GET /not-exist HTTP/1.0\r\n\n", 'HTTP/1.1 404', 'Not existent file');
mkdir $test_dir . $dir_separator . 'x';
my $path = $test_dir . $dir_separator . 'x' . $dir_separator . 'index.cgi';
write_file($path, read_file($root . $dir_separator . 'env.cgi'));
chmod(0755, $path);
o("GET /$test_dir_uri/x/ HTTP/1.0\n\n", "Content-Type: text/html\r\n\r\n",
  'index.cgi execution');
o("GET /$test_dir_uri/x/ HTTP/1.0\n\n",
  "SCRIPT_FILENAME=test/test_dir/x/index.cgi", 'SCRIPT_FILENAME');
o("GET /ta/x/ HTTP/1.0\n\n", "SCRIPT_NAME=/ta/x/index.cgi",
  'Aliases SCRIPT_NAME');
o("GET /hello.txt HTTP/1.1\nConnection: close\n\n", 'Connection: close',
  'No keep-alive');

$path = $test_dir . $dir_separator . 'x' . $dir_separator . 'a.cgi';
system("ln -s `which perl` $root/myperl") == 0 or fail("Can't symlink perl");
write_file($path, "#!../../myperl\n" .
           "print \"Content-Type: text/plain\\n\\nhi\";");
chmod(0755, $path);
o("GET /$test_dir_uri/x/a.cgi HTTP/1.0\n\n", "hi", 'Relative CGI interp path');
o("GET * HTTP/1.0\n\n", "^HTTP/1.1 404", '* URI');

my $mime_types = {
  html => 'text/html',
  htm => 'text/html',
  txt => 'text/plain',
  unknown_extension => 'text/plain',
  js => 'application/x-javascript',
  css => 'text/css',
  jpg => 'image/jpeg',
  c => 'text/plain',
  'tar.gz' => 'blah',
  bar => 'foo/bar',
  baz => 'foo',
};

foreach my $key (keys %$mime_types) {
  my $filename = "_mime_file_test.$key";
  write_file("$root/$filename", '');
  o("GET /$filename HTTP/1.0\n\n",
    "Content-Type: $mime_types->{$key}", ".$key mime type");
  unlink "$root/$filename";
}

# Get binary file and check the integrity
my $binary_file = 'binary_file';
my $f2 = '';
foreach (0..123456) { $f2 .= chr(int(rand() * 255)); }
write_file("$root/$binary_file", $f2);
my $f1 = req("GET /$binary_file HTTP/1.0\r\n\n");
while ($f1 =~ /^.*\r\n/) { $f1 =~ s/^.*\r\n// }
$f1 eq $f2 or fail("Integrity check for downloaded binary file");

my $range_request = "GET /hello.txt HTTP/1.1\nConnection: close\n".
"Range: bytes=3-5\r\n\r\n";
o($range_request, '206 Partial Content', 'Range: 206 status code');
o($range_request, 'Content-Length: 3\s', 'Range: Content-Length');
o($range_request, 'Content-Range: bytes 3-5/17', 'Range: Content-Range');
o($range_request, '\nple$', 'Range: body content');

# Test directory sorting. Sleep between file creation for 1.1 seconds,
# to make sure modification time are different.
mkdir "$test_dir/sort";
write_file("$test_dir/sort/11", 'xx');
select undef, undef, undef, 1.1;
write_file("$test_dir/sort/aa", 'xxxx');
select undef, undef, undef, 1.1;
write_file("$test_dir/sort/bb", 'xxx');
select undef, undef, undef, 1.1;
write_file("$test_dir/sort/22", 'x');

o("GET /$test_dir_uri/sort/?n HTTP/1.0\n\n",
  '200 OK.+>11<.+>22<.+>aa<.+>bb<',
  'Directory listing (name, ascending)');
o("GET /$test_dir_uri/sort/?nd HTTP/1.0\n\n",
  '200 OK.+>bb<.+>aa<.+>22<.+>11<',
  'Directory listing (name, descending)');
o("GET /$test_dir_uri/sort/?s HTTP/1.0\n\n",
  '200 OK.+>22<.+>11<.+>bb<.+>aa<',
  'Directory listing (size, ascending)');
o("GET /$test_dir_uri/sort/?sd HTTP/1.0\n\n",
  '200 OK.+>aa<.+>bb<.+>11<.+>22<',
  'Directory listing (size, descending)');
o("GET /$test_dir_uri/sort/?d HTTP/1.0\n\n",
  '200 OK.+>11<.+>aa<.+>bb<.+>22<',
  'Directory listing (modification time, ascending)');
o("GET /$test_dir_uri/sort/?dd HTTP/1.0\n\n",
  '200 OK.+>22<.+>bb<.+>aa<.+>11<',
  'Directory listing (modification time, descending)');

unless (scalar(@ARGV) > 0 and $ARGV[0] eq "basic_tests") {
  # Check that .htpasswd file existence trigger authorization
  write_file("$root/.htpasswd", 'user with space, " and comma:mydomain.com:5deda12442309cbdcdffc6b2737a894f');
  o("GET /hello.txt HTTP/1.1\n\n", '401 Unauthorized',
    '.htpasswd - triggering auth on file request');
  o("GET / HTTP/1.1\n\n", '401 Unauthorized',
    '.htpasswd - triggering auth on directory request');

  # Test various funky things in an authentication header.
  o("GET /hello.txt HTTP/1.0\nAuthorization: Digest   eq== empty=\"\", empty2=, quoted=\"blah foo bar, baz\\\"\\\" more\\\"\", unterminatedquoted=\" doesn't stop\n\n",
    '401 Unauthorized', 'weird auth values should not cause crashes');
  my $auth_header = "Digest username=\"user with space, \\\" and comma\", ".
    "realm=\"mydomain.com\", nonce=\"1291376417\", uri=\"/\",".
    "response=\"e8dec0c2a1a0c8a7e9a97b4b5ea6a6e6\", qop=auth, nc=00000001, cnonce=\"1a49b53a47a66e82\"";
  o("GET /hello.txt HTTP/1.0\nAuthorization: $auth_header\n\n", 'HTTP/1.1 200 OK', 'GET regular file with auth');
  o("GET / HTTP/1.0\nAuthorization: $auth_header\n\n", '^(.(?!(.htpasswd)))*$',
    '.htpasswd is hidden from the directory list');
  o("GET / HTTP/1.0\nAuthorization: $auth_header\n\n", '^(.(?!(exploit.pl)))*$',
    'hidden file is hidden from the directory list');
  o("GET /.htpasswd HTTP/1.0\nAuthorization: $auth_header\n\n",
    '^HTTP/1.1 404 ', '.htpasswd must not be shown');
  o("GET /exploit.pl HTTP/1.0\nAuthorization: $auth_header\n\n",
    '^HTTP/1.1 404', 'hidden files must not be shown');
  unlink "$root/.htpasswd";


  o("GET /env.cgi HTTP/1.0\n\r\n", 'HTTP/1.1 200 OK', 'GET CGI file');
  o("GET /bad2.cgi HTTP/1.0\n\n", "HTTP/1.1 123 Please pass me to the client\r",
    'CGI Status code text');
  o("GET /sh.cgi HTTP/1.0\n\r\n", 'shell script CGI',
    'GET sh CGI file') unless on_windows();
  o("GET /env.cgi?var=HELLO HTTP/1.0\n\n", 'QUERY_STRING=var=HELLO',
    'QUERY_STRING wrong');
  o("POST /env.cgi HTTP/1.0\r\nContent-Length: 9\r\n\r\nvar=HELLO",
    'var=HELLO', 'CGI POST wrong');
  o("POST /env.cgi HTTP/1.0\r\nContent-Length: 9\r\n\r\nvar=HELLO",
    '\x0aCONTENT_LENGTH=9', 'Content-Length not being passed to CGI');
  o("GET /env.cgi HTTP/1.0\nMy-HdR: abc\n\r\n",
    'HTTP_MY_HDR=abc', 'HTTP_* env');
  o("GET /env.cgi HTTP/1.0\n\r\nSOME_TRAILING_DATA_HERE",
    'HTTP/1.1 200 OK', 'GET CGI with trailing data');

  o("GET /env.cgi%20 HTTP/1.0\n\r\n",
    'HTTP/1.1 404', 'CGI Win32 code disclosure (%20)');
  o("GET /env.cgi%ff HTTP/1.0\n\r\n",
    'HTTP/1.1 404', 'CGI Win32 code disclosure (%ff)');
  o("GET /env.cgi%2e HTTP/1.0\n\r\n",
    'HTTP/1.1 404', 'CGI Win32 code disclosure (%2e)');
  o("GET /env.cgi%2b HTTP/1.0\n\r\n",
    'HTTP/1.1 404', 'CGI Win32 code disclosure (%2b)');
  o("GET /env.cgi HTTP/1.0\n\r\n", '\nHTTPS=off\n', 'CGI HTTPS');
  o("GET /env.cgi HTTP/1.0\n\r\n", '\nCGI_FOO=foo\n', '-cgi_env 1');
  o("GET /env.cgi HTTP/1.0\n\r\n", '\nCGI_BAR=bar\n', '-cgi_env 2');
  o("GET /env.cgi HTTP/1.0\n\r\n", '\nCGI_BAZ=baz\n', '-cgi_env 3');
  o("GET /env.cgi/a/b/98 HTTP/1.0\n\r\n", 'PATH_INFO=/a/b/98\n', 'PATH_INFO');
  o("GET /env.cgi/a/b/9 HTTP/1.0\n\r\n", 'PATH_INFO=/a/b/9\n', 'PATH_INFO');

  # Check that CGI's current directory is set to script's directory
  my $copy_cmd = on_windows() ? 'copy' : 'cp';
  system("$copy_cmd $root" . $dir_separator .  "env.cgi $test_dir" .
    $dir_separator . 'env.cgi');
  o("GET /$test_dir_uri/env.cgi HTTP/1.0\n\n",
    "CURRENT_DIR=.*$root/$test_dir_uri", "CGI chdir()");

  # SSI tests
  o("GET /ssi1.shtml HTTP/1.0\n\n",
    'ssi_begin.+CFLAGS.+ssi_end', 'SSI #include file=');
  o("GET /ssi2.shtml HTTP/1.0\n\n",
    'ssi_begin.+Unit test.+ssi_end', 'SSI #include virtual=');
  my $ssi_exec = on_windows() ? 'ssi4.shtml' : 'ssi3.shtml';
  o("GET /$ssi_exec HTTP/1.0\n\n",
    'ssi_begin.+Makefile.+ssi_end', 'SSI #exec');
  my $abs_path = on_windows() ? 'ssi6.shtml' : 'ssi5.shtml';
  my $word = on_windows() ? 'boot loader' : 'root';
  o("GET /$abs_path HTTP/1.0\n\n",
    "ssi_begin.+$word.+ssi_end", 'SSI #include file= (absolute)');
  o("GET /ssi7.shtml HTTP/1.0\n\n",
    'ssi_begin.+Unit test.+ssi_end', 'SSI #include "..."');
  o("GET /ssi8.shtml HTTP/1.0\n\n",
    'ssi_begin.+CFLAGS.+ssi_end', 'SSI nested #includes');

  # Manipulate the passwords file
  my $path = 'test_htpasswd';
  unlink $path;
  system("$exe -A $path a b c") == 0
    or fail("Cannot add user in a passwd file");
  system("$exe -A $path a b c2") == 0
    or fail("Cannot edit user in a passwd file");
  my $content = read_file($path);
  $content =~ /^b:a:\w+$/gs or fail("Bad content of the passwd file");
  unlink $path;

  do_PUT_test();
  kill_spawned_child();
  do_unit_test();
  do_embedded_test();
}

sub do_PUT_test {
  # This only works because mongoose currently doesn't look at the nonce.
  # It should really be rejected...
  my $auth_header = "Authorization: Digest  username=guest, ".
  "realm=mydomain.com, nonce=1145872809, uri=/put.txt, ".
  "response=896327350763836180c61d87578037d9, qop=auth, ".
  "nc=00000002, cnonce=53eddd3be4e26a98\n";

  o("PUT /a/put.txt HTTP/1.0\nContent-Length: 7\n$auth_header\n1234567",
    "HTTP/1.1 201 OK", 'PUT file, status 201');
  fail("PUT content mismatch")
  unless read_file("$root/a/put.txt") eq '1234567';
  o("PUT /a/put.txt HTTP/1.0\nContent-Length: 4\n$auth_header\nabcd",
    "HTTP/1.1 200 OK", 'PUT file, status 200');
  fail("PUT content mismatch")
  unless read_file("$root/a/put.txt") eq 'abcd';
  o("PUT /a/put.txt HTTP/1.0\n$auth_header\nabcd",
    "HTTP/1.1 411 Length Required", 'PUT 411 error');
  o("PUT /a/put.txt HTTP/1.0\nExpect: blah\nContent-Length: 1\n".
    "$auth_header\nabcd",
    "HTTP/1.1 417 Expectation Failed", 'PUT 417 error');
  o("PUT /a/put.txt HTTP/1.0\nExpect: 100-continue\nContent-Length: 4\n".
    "$auth_header\nabcd",
    "HTTP/1.1 100 Continue.+HTTP/1.1 200", 'PUT 100-Continue');
}

sub do_unit_test {
  my $cmd = "cc -g -W -Wall -o $unit_test_exe $root/unit_test.c -I. ".
    "-pthread -DNO_SSL ";
  if (on_windows()) {
    $cmd = "cl $root/embed.c mongoose.c /I. /nologo /DNO_SSL ".
    "/DLISTENING_PORT=\\\"$port\\\" /link /out:$embed_exe.exe ws2_32.lib ";
  }
  print $cmd, "\n";
  system($cmd) == 0 or fail("Cannot compile unit test");
  system($unit_test_exe) == 0 or fail("Unit test failed!");
}

sub do_embedded_test {
  my $cmd = "cc -W -Wall -o $embed_exe $root/embed.c mongoose.c -I. ".
  "-pthread -DNO_SSL -DLISTENING_PORT=\\\"$port\\\"";
  if (on_windows()) {
    $cmd = "cl $root/embed.c mongoose.c /I. /nologo /DNO_SSL ".
    "/DLISTENING_PORT=\\\"$port\\\" /link /out:$embed_exe.exe ws2_32.lib ";
  }
  print $cmd, "\n";
  system($cmd) == 0 or fail("Cannot compile embedded unit test");

  spawn("./$embed_exe");
  o("GET /test_get_header HTTP/1.0\nHost: blah\n\n",
    'Value: \[blah\]', 'mg_get_header', 0);
  o("GET /test_get_var?a=b&my_var=foo&c=d HTTP/1.0\n\n",
    'Value: \[foo\]', 'mg_get_var 1', 0);
  o("GET /test_get_var?my_var=foo&c=d HTTP/1.0\n\n",
    'Value: \[foo\]', 'mg_get_var 2', 0);
  o("GET /test_get_var?a=b&my_var=foo HTTP/1.0\n\n",
    'Value: \[foo\]', 'mg_get_var 3', 0);
  o("POST /test_get_var HTTP/1.0\nContent-Length: 10\n\n".
    "my_var=foo", 'Value: \[foo\]', 'mg_get_var 4', 0);
  o("POST /test_get_var HTTP/1.0\nContent-Length: 18\n\n".
    "a=b&my_var=foo&c=d", 'Value: \[foo\]', 'mg_get_var 5', 0);
  o("POST /test_get_var HTTP/1.0\nContent-Length: 14\n\n".
    "a=b&my_var=foo", 'Value: \[foo\]', 'mg_get_var 6', 0);
  o("GET /test_get_var?a=one%2btwo&my_var=foo& HTTP/1.0\n\n",
    'Value: \[foo\]', 'mg_get_var 7', 0);
  o("GET /test_get_var?my_var=one%2btwo&b=two%2b HTTP/1.0\n\n",
    'Value: \[one\+two\]', 'mg_get_var 8', 0);

  # + in form data MUST be decoded to space
  o("POST /test_get_var HTTP/1.0\nContent-Length: 10\n\n".
    "my_var=b+c", 'Value: \[b c\]', 'mg_get_var 9', 0);

  # Test that big POSTed vars are not truncated
  my $my_var = 'x' x 64000;
  o("POST /test_get_var HTTP/1.0\nContent-Length: 64007\n\n".
    "my_var=$my_var", 'Value size: \[64000\]', 'mg_get_var 10', 0);

  # Other methods should also work
  o("PUT /test_get_var HTTP/1.0\nContent-Length: 10\n\n".
    "my_var=foo", 'Value: \[foo\]', 'mg_get_var 11', 0);

  o("POST /test_get_request_info?xx=yy HTTP/1.0\nFoo: bar\n".
    "Content-Length: 3\n\na=b",
    'Method: \[POST\].URI: \[/test_get_request_info\].'.
    'HTTP version: \[1.0\].HTTP header \[Foo\]: \[bar\].'.
    'HTTP header \[Content-Length\]: \[3\].'.
    'Query string: \[xx=yy\].'.
    'Remote IP: \[\d+\].Remote port: \[\d+\].'.
    'Remote user: \[\]'
    , 'request_info', 0);
  o("GET /not_exist HTTP/1.0\n\n", 'Error: \[404\]', '404 handler', 0);
  o("bad request\n\n", 'Error: \[400\]', '* error handler', 0);
#	o("GET /foo/secret HTTP/1.0\n\n",
#		'401 Unauthorized', 'mg_protect_uri', 0);
#	o("GET /foo/secret HTTP/1.0\nAuthorization: Digest username=bill\n\n",
#		'401 Unauthorized', 'mg_protect_uri (bill)', 0);
#	o("GET /foo/secret HTTP/1.0\nAuthorization: Digest username=joe\n\n",
#		'200 OK', 'mg_protect_uri (joe)', 0);

  kill_spawned_child();
}

print "SUCCESS! All tests passed.\n";
