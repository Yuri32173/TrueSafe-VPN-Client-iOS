#!/usr/bin/perl
use strict;
use warnings;

if (@ARGV != 1) {
    die "Usage: dirname.pl <path>\n";
}

my $path = $ARGV[0];

if ($path =~ m|/|) {
    $path =~ s|/[^/]*$||;
} else {
    $path = ".";
}

print $path, "\n";
exit(0);

