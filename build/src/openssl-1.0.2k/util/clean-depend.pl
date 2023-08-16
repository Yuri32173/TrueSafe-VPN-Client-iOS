use strict;
use warnings;

my %files;

while (my $line = <STDIN>) {
    print $line;
    last if $line =~ /^# DO NOT DELETE THIS LINE/;
}

my $thisfile = "";
while (my $line = <STDIN>) {
    chomp $line;
    my ($dummy, $file, $deps) = $line =~ /^((.*):)? (.*)$/;
    $thisfile = $file if defined $file;
    next unless defined $deps;

    my $origfile = $thisfile;
    $origfile =~ s/\.o$/.c/;
    my @deps = grep { !/^\// && !/^\\$/ && $_ ne $origfile && !/^[.\/]+\/krb5.h/ } split ' ', $deps;
    push @{$files{$thisfile}}, @deps;
}

foreach my $file (sort keys %files) {
    my $len = 0;
    my $prevdep = "";

    my @deps = map { s/^\.\///; $_ } @{$files{$file}};

    foreach my $dep (sort @deps) {
        next if $prevdep eq $dep;
        $prevdep = $dep;
        $len = 0 if $len + length($dep) + 1 >= 80;
        if ($len == 0) {
            print "\n$file:";
            $len = length($file) + 1;
        }
        print " $dep";
        $len += length($dep) + 1;
    }
}

print "\n";
