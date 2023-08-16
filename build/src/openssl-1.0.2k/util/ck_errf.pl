#!/usr/local/bin/perl

use strict;
use warnings;

my $config = "crypto/err/openssl.ec";
my $hprefix = "openssl/";
my $debug = 0;
my $rebuild = 0;
my $static = 1;
my $recurse = 0;
my $reindex = 0;
my $dowrite = 0;
my $staticloader = "";

my $pack_errcode;
my $load_errcode;

my $errcount;
my $year = (localtime)[5] + 1900;

while (@ARGV) {
    my $arg = shift @ARGV;
    if ($arg eq "-conf") {
        $config = shift @ARGV;
    } elsif ($arg eq "-hprefix") {
        $hprefix = shift @ARGV;
    } elsif ($arg eq "-debug") {
        $debug = 1;
    } elsif ($arg eq "-rebuild") {
        $rebuild = 1;
    } elsif ($arg eq "-recurse") {
        $recurse = 1;
    } elsif ($arg eq "-reindex") {
        $reindex = 1;
    } elsif ($arg eq "-nostatic") {
        $static = 0;
    } elsif ($arg eq "-staticloader") {
        $staticloader = "static ";
    } elsif ($arg eq "-write") {
        $dowrite = 1;
    } elsif ($arg eq "-help" || $arg eq "-h" || $arg eq "-?" || $arg eq "--help") {
        print_help();
        exit 1;
    } else {
        unshift @ARGV, $arg;
        last;
    }
}

sub print_help {
    print STDERR <<"EOF";
mkerr.pl [options] ...

Options:

...

  ...           Additional arguments are added to the file list to scan,
                assuming '-recurse' was NOT specified on the command line.

EOF
}

my @source;
if ($recurse) {
    @source = (<crypto/*.c>, <crypto/*/*.c>, <ssl/*.c>);
} else {
    @source = @ARGV;
}
