#!/usr/bin/env perl
use strict;
use warnings;

my $num = 1;
my $width = 40;

while (<>) {
    chomp;
    my $i = length($_);
    my $n = $width - $i;
    my $tabs = int(($n + 7) / 8);
    print $_ . ("\t" x $tabs) . $num . "\n";
    $num++;
}
