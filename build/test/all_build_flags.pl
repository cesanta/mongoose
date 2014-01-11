#!/usr/bin/env perl
use Data::Dumper;
@flags = split /\s+/,
  `egrep -o 'NO_[A-Z0-9_]*|USE_[A-Z0-9_]*' ../mongoose.c  | sort | uniq`;
#@flags = ("NO_POPEN", "NO_SSL", "NDEBUG", "DEBUG", "NO_CGI");
my $num_flags = @flags;

sub fail {
  print "FAILED: @_\n";
  exit 1;
}

my $extra = {
  'USE_SSL' => '-lssl',
  'USE_LUA' => 'lua_5.2.1.c',
};

#my $platform = $ARGV[0] || "linux";

for (my $i = 0; $i < 2 ** $num_flags; $i++) {
  my $bitmask = sprintf("%*.*b", $num_flags, $num_flags, $i);
  my @combination = ();
  for (my $j = 0; $j < $num_flags; $j++) {
    next unless substr($bitmask, $j, 1);
    my $def = $flags[$j];
    next if $def eq 'USE_LUA_SQLITE3';
    push @combination, "-D$def";
    push @combination, $extra->{$def} if $extra->{$def};
  }
  my $defines = join(" ", @combination);
  my $cmd = "make clean all CFLAGS_EXTRA=\"-O0 $defines\"";
  #print "Testing [$defines]\n";
  system($cmd) == 0 or fail "build failed: $_";
  #system("perl test/test.pl basic_tests >/dev/null") == 0
  #  or fail "basic tests";
  #print "Basic tests: OK\n";
}

print "PASS: All builds passed!\n";
