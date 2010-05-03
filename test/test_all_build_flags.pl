#!/usr/bin/env perl

@flags = ("NO_SSI", "NO_SSL", "NDEBUG", "DEBUG", "NO_CGI");
my $num_flags = @flags;

sub fail {
	print "FAILED: @_\n";
	exit 1;
}

my $platform = $ARGV[0] || "linux";

for (my $i = 0; $i < 2 ** $num_flags; $i++) {
	my $bitmask = sprintf("%*.*b", $num_flags, $num_flags, $i);
	my @combination = ();
	for (my $j = 0; $j < $num_flags; $j++) {
		push @combination, $flags[$j] if substr($bitmask, $j, 1);
	}
	my $defines = join(" ", map { "-D$_" } @combination);
	my $cmd = "CFLAGS=\"$defines\" make clean $platform >/dev/null";
	system($cmd) == 0 or fail "build failed: $_";
	print "Build succeeded, flags: [$defines]\n";
	system("perl test/test.pl basic_tests >/dev/null") == 0
		or fail "basic tests";
	print "Basic tests: OK\n";
}

print "PASS: All builds passed!\n";
