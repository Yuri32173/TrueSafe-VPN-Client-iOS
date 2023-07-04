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
    if (/^LIBRARY\s+cryptssl\b/) {
      s/\b(cryptssl)\b/crypto/;
      $did_library = $1;
    }
    elsif (/^LIBRARY\s+open_ssl\b/) {
      s/\b(open_ssl)\b/ssl/;
      $did_library = $1;
    }
  }

  unless ($did_description) {
    if (/^(DESCRIPTION\s+(['"])).*/) {
      $_ = $1 . '@#www.openssl.org/:#@forwarder DLL for pre-0.9.7c+ OpenSSL to the new dll naming scheme' . $2;
      $did_description++;
    }
  }

  if ($do_exports) {
    last unless /\S/;
    unless (/^ \s* ( \w+ ) \s+ \@(\d+)\s*$/x) {
      warn;
      last;
    }
    push @imports, [$1, $2];
    $_ .= " NONAME" if $noname;
  }

  $do_exports++ if not $do_exports and /^EXPORTS/;
  print $_;
}

print "IMPORTS\n";
for my $imp (@imports) {
  print "\t$imp->[0]=$did_library.$imp->[1]\n";
}
