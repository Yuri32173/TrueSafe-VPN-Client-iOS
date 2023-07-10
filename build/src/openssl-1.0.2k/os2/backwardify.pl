#!/usr/bin/perl
use strict;
use warnings;

my $did_library;
my $did_description;
my $do_exports;
my @imports;
my $noname = (@ARGV and $ARGV[0] eq '-noname' and shift);

while (<>) {
  unless ($did_library) {
    $did_library = $1 if /^LIBRARY\s+(cryptssl|open_ssl)\b/;
    s/\b(cryptssl|open_ssl)\b/crypto/ if $did_library;
  }

  unless ($did_description) {
    $did_description++ if s/^(DESCRIPTION\s+(['"])).*/${1}\@#www.openssl.org/:#\@forwarder DLL for pre-0.9.7c+ OpenSSL to the new dll naming scheme$2/;
  }

  if ($do_exports) {
    last unless /\S/;
    if (/^ \s* ( \w+ ) \s+ \@(\d+)\s*$/x) {
      push @imports, [$1, $2];
      s/$/ NONAME/ if $noname;
    } else {
      warn;
      last;
    }
  }

  $do_exports++ if not $do_exports and /^EXPORTS/;
  print;
}

print "IMPORTS\n";
for my $imp (@imports) {
  print "\t$imp->[0]=$did_library.$imp->[1]\n";
}
