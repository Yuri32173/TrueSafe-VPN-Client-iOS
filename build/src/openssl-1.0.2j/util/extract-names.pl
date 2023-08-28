#!/usr/bin/perl

use strict;
use warnings;

my $paragraph_separator = ""; # Set the input record separator to empty to read paragraphs
my @words;
my $name = 0;

while (<STDIN>) {
    chomp;
    s/\n/ /gm;

    if (/^=head1 /) {
        $name = 0;
    } elsif ($name) {
        if (/ - /) {
            s/ - .*//;
            s/,\s+/,/g;
            s/\s+,/,/g;
            s/^\s+//g;
            s/\s+$//g;
            s/\s/_/g;
            push @words, split ',';
        }
    }

    if (/^=head1 *NAME *$/) {
        $name = 1;
    }
}

print join("\n", @words), "\n";
