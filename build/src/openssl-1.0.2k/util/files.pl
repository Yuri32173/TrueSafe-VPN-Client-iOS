#!/usr/local/bin/perl

use strict;
use warnings;

# used to generate the file MINFO for use by util/mk1mf.pl
# It is basically a list of all variables from the passed makefile

my %sym;
my $s = "";
my $dir = "";

while ($ARGV[0] =~ /^(\S+)\s*=(.*)$/) {
    $sym{$1} = $2;
    shift;
}

while (<>) {
    chomp;
    s/#.*//;
    if (/^(\S+)\s*=\s*(.*)$/) {
        my ($s, $b) = ($1, $2);
        my $o = "";
        while ($b =~ /\\$/) {
            chop($b);
            $o .= $b . " ";
            $b = <>;
            chop($b);
        }
        $o .= $b . " ";
        $o =~ s/^\s+//;
        $o =~ s/\s+$//;
        $o =~ s/\s+/ /g;

        $o =~ s/\$[({]([^)}]+)[)}]/$sym{$1}/g;
        $sym{$s} = $o if !exists $sym{$s};
    }
}

my $pwd = `pwd`;
chomp($pwd);

if ($sym{'TOP'} eq ".") {
    $dir = ".";
} else {
    my @pwd_parts = split(/\//, $pwd);
    my @top_parts = split(/\//, $sym{'TOP'});
    my $z = $#pwd_parts - $#top_parts + 1;
    foreach my $i ($z .. $#pwd_parts) {
        $dir .= $pwd_parts[$i] . "/";
    }
    chop($dir);
}

print "RELATIVE_DIRECTORY=$dir\n";

foreach my $key (sort keys %sym) {
    print "$key=$sym{$key}\n";
}
print "RELATIVE_DIRECTORY=\n";
