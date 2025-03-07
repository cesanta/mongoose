#!/usr/bin/perl

# Copyright (c) 2016, Freescale Semiconductor, Inc.
# Copyright 2016 NXP
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# o Redistributions of source code must retain the above copyright notice, this list
#   of conditions and the following disclaimer.
#
# o Redistributions in binary form must reproduce the above copyright notice, this
#   list of conditions and the following disclaimer in the documentation and/or
#   other materials provided with the distribution.
#
# o Neither the name of the copyright holder nor the names of its
#   contributors may be used to endorse or promote products derived from this
#   software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Perl script to generate source code from static web pages - generates static file system.
#
# Run script from directory containing data directory! Script recursively searches files 
# in data subdirectory. Script generates file httpsrv_fs_data.c which contains converted data 
# (pages, pictures, ...) in C constant arrays. Separate C files can be created for selected
# input files.
#
# Perl:
# 	perl mkfs.pl -s <separate_file> <input directory>
#
# 	Example: perl mkfs.pl -s image.bmp my_web


use File::Find;
use File::Compare;


# Get input

%SEPARATE_FILES = ();
$INPUT_DIR = "";
while(@ARGV)
{
  if ($ARGV[0] =~ /^-s$/)
  {
    shift @ARGV;
    if (@ARGV == 0) { last; }
    $tmp = $ARGV[0];
    $tmp =~ s#\\#/#g;
    $SEPARATE_FILES{$tmp} = 1;
  } else {
    $INPUT_DIR = $ARGV[0];
    shift @ARGV;
    last;
  }
  shift @ARGV;
}

# Check input

if (($INPUT_DIR =~ /^$/) || (@ARGV))
{
  my $readme =
      "\tThis tool creates C language source file httpsrv_fs_data.c with\n".
      "\tconstant arrays of binary data of all input directory files.\n".
      "\tData of selected files can be stored in separate C files.\n";

  print "$readme\n";
  print "Usage:\n";
  print "mkfs.pl [-s <separate_file>] <input_directory>\n";
  exit(0);
}

# Gather files

@INPUT_FILES = ();
find (\&get_files, $INPUT_DIR);

# Open httpsrv_fs_data.tmp for writing

open(OUTPUT, "> httpsrv_fs_data.tmp") or die "Can't create temporary file httpsrv_fs_data.tmp!\n";
print(OUTPUT "#include <httpsrv_fs.h>\n\n");
print(OUTPUT "extern const HTTPSRV_FS_DIR_ENTRY httpsrv_fs_data[];\n\n");
foreach $file (@INPUT_FILES)
{
  $fvar = "httpsrv_fs_" . $file;
  $fvar =~ s#[/\.]#_#g;
  if ($SEPARATE_FILES{$file})
  {
    $size = -s $file;
    print(OUTPUT "extern const unsigned char " . $fvar . "[$size];\n\n");
  }
}

# Process input files

foreach $file (@INPUT_FILES)
{
  print "Processing file $file\n";
  $fvar = "httpsrv_fs_" . $file;
  $fvar =~ s#[/\.]#_#g;
  &process_file ($file, $fvar, $SEPARATE_FILES{$file});
}

# Finish httpsrv_fs_data.tmp file

print(OUTPUT "const HTTPSRV_FS_DIR_ENTRY httpsrv_fs_data[] = {\n");
foreach $file (@INPUT_FILES)
{
  $fvar = "httpsrv_fs_" . $file;
  $fvar =~ s#[/\.]#_#g;
  $dest = $file;
  $dest =~ s/^$INPUT_DIR//;
  print(OUTPUT "\t{ \"${dest}\", 0, ");
  print(OUTPUT "(unsigned char*)${fvar}, sizeof(${fvar}) },\n");
}
print(OUTPUT "\t{ 0, 0, 0, 0 }\n};\n\n");
close(OUTPUT);

# Rename temporary to *.c files

$SEPARATE_FILES{"data"} = 1;
@INPUT_FILES = (@INPUT_FILES, "data");
foreach $file (@INPUT_FILES)
{
  $fvar = "httpsrv_fs_" . $file;
  $fvar =~ s#[/\.]#_#g;
  if ($SEPARATE_FILES{$file})
  {
    if (compare("${fvar}.tmp", "${fvar}.c") == 0)
    {
      unlink("${fvar}.tmp");
    } else {
      if (&check_write_protect("${fvar}.c"))
      {
        rename("${fvar}.tmp", "${fvar}.c");
      } else {
        unlink("${fvar}.tmp");
      }
    }
  }
}
print "Done.\n";
exit(0);


################################################################################


sub get_files
{
  if ((-f $_) && ($File::Find::name !~ /(CVS|~)/))
  {
    $tmp = $File::Find::name;
    $tmp =~ s#^/##;
    @INPUT_FILES = (@INPUT_FILES, $tmp);
  }
}


sub check_write_protect
{
  my ($file) = @_;
  
  if (-e $file && ! -w $file)
  {
    print "Overwriting read only file $file, are you sure? ";
    $response = <STDIN>;
    chomp($response);
    if ($response !~ /[Yy]/)
    {
      return 0;
    }
    system("attrib -R $file");
  }
  return 1;
}


sub process_file
{
  my ($file, $fvar, $separate) = @_;

  open(FILE, $file) or die "Can't open file ${file}!\n";

  binmode(FILE);

  $output = "OUTPUT";
  if ($separate)
  {
    $output = "SEPARATE";
    open($output, "> ${fvar}.tmp") or die "Can't create temporary file ${fvar}.tmp!\n";
    print($output "extern const unsigned char " . $fvar . "[];\n\n");
    print($output "const unsigned char " . $fvar . "[] = {\n");
  } else {
    print($output "static const unsigned char " . $fvar . "[] = {\n");
  }

  print($output "\t/* $file */\n");

  $sep = "\t";
  while(read(FILE, $data, 10, 0))
  {
    foreach(split(//, $data))
    {
      printf($output "${sep}0x%02x", ord($_));
      $sep = ", ";
    }
    $sep = ",\n\t";
  }

  print($output "\n};\n\n");

  if ($separate)
  {
    close($output);
  }
  
  close(FILE);
}

