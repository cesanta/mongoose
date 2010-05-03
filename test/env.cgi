#!/usr/bin/env perl

use Cwd;
use CGI;

use vars '%in';
CGI::ReadParse();

print "Content-Type: text/html\r\n\r\n";

print "<pre>\n";
foreach my $key (sort keys %ENV) {
	print "$key=$ENV{$key}\n";
}

print "\n";

foreach my $key (sort keys %in) {
	print "$key=$in{$key}\n";
}

print "\n";

#sleep 10;

print 'CURRENT_DIR=' . getcwd() . "\n";
print "</pre>\n";

my $stuff = <<EOP ;
<script language="javascript">
	function set_val() {
	}
</script>
<form method=get>
	<input type=hidden name=a>
	<input type=text name=_a onChange="javascript: this.form.a.value=this.value;">
	<input type=submit value=get>
</form>

<form method=post>
	<input type=text name=b>
	<input type=submit value=post>
</form>
EOP

system('some shit');

#print STDERR "fuck!!!\n\n";

print $stuff;
