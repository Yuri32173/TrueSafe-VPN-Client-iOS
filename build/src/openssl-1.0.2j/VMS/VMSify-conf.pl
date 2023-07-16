#!/usr/bin/perl

use strict;
use warnings;

my @directory_vars = ("dir", "certs", "crl_dir", "new_certs_dir");
my @file_vars = ("database", "certificate", "serial", "crlnumber",
                 "crl", "private_key", "RANDFILE");

while (my $line = <STDIN>) {
    chomp $line;

    foreach my $d (@directory_vars) {
        if ($line =~ /^(\s*#?\s*$d\s*=\s*)\.\/([^\s#]*)([\s#].*)$/) {
            $line = "$1sys\\\$disk:\\[$2$3";
        } elsif ($line =~ /^(\s*#?\s*$d\s*=\s*)(\w[^\s#]*)([\s#].*)$/) {
            $line = "$1sys\\\$disk:\\[$2$3";
        }
        $line =~ s/^(\s*#?\s*$d\s*=\s*\$\w+)\/([^\s#]*)([\s#].*)$/$1.$2]$3/;

        while ($line =~ /^(\s*#?\s*$d\s*=\s*(\$\w+\.|sys\\\$disk:\\\[.)[\w.]+)\/([^\]]*)\](.*)$/) {
            $line = "$1.$3]$4";
        }
    }

    foreach my $f (@file_vars) {
        $line =~ s/^(\s*#?\s*$f\s*=\s*)\.\/(.*)$/$1sys\\\$disk:\\[\/$2/;

        while ($line =~ /^(\s*#?\s*$f\s*=\s*(\$\w+|sys\\\$disk:\\\[)\/[^\/]*)\/(\w+\/[^\s#]*)([\s#].*)$/) {
            $line = "$1.$3$4";
        }

        if ($line =~ /^(\s*#?\s*$f\s*=\s*(\$\w+|sys\\\$disk:\\\[)\/(\w+))([\s#].*)$/) {
            $line = "$1]$3.$4";
        } elsif ($line =~ /^(\s*#?\s*$f\s*=\s*(\$\w+|sys\\\$disk:\\\[)\/([^\s#]*))([\s#].*)$/) {
            $line = "$1]$3$4";
        }
    }

    print $line, "\n";
}
