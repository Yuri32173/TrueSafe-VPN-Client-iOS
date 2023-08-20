#!/usr/bin/perl

$/ = "";  # Eat a paragraph at once.

my $name_section = 0;
my @words;

while (<STDIN>) {
    chop;
    s/\n/ /gm;

    if (/^=head1 /) {
        $name_section = 0;
    } elsif ($name_section) {
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
        $name_section = 1;
    }
}

print join("\n", @words), "\n";
