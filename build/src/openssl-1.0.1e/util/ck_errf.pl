#!/usr/local/bin/perl
#
# This is just a quick script to scan for cases where the 'error'
# function name in a XXXerr() macro is wrong.
# 
# Run in the top level by going
# perl util/ck_errf.pl */*.c */*/*.c
#

my $err_strict = 0;
my $bad = 0;

foreach my $file (@ARGV) {
    if ($file eq "-strict") {
        $err_strict = 1;
        next;
    }
    open(my $IN, "<", $file) || die "unable to open $file: $!\n";
    my $func = "";
    while (<$IN>) {
        if (!/;$/ && /^([a-zA-Z].*[\s*])?([A-Za-z_0-9]+)\(.*[),]/) {
            /^([^()]*(\([^()]*\)[^()]*)*)\(/;
            my $match = $1;
            $match =~ /([A-Za-z_0-9]*)$/;
            $func = lc $1;
        }
        if (/([A-Z0-9]+)err\(([^,]+)/ && !/ckerr_ignore/) {
            my $errlib = $1;
            my $n = $2;

            if ($func eq "") {
                print "$file:$.:???:$n\n";
                $bad = 1;
                next;
            }

            if ($n !~ /([^_]+)_F_(.+)$/) {
                next;
            }
            my $lib = $1;
            $n = lc $2;

            if ($lib ne $errlib) {
                print "$file:$.:$func:$n [${errlib}err]\n";
                $bad = 1;
                next;
            }

            if (($n ne $func) && ($errlib ne "SYS")) {
                print "$file:$.:$func:$n\n";
                $bad = 1;
                next;
            }
        }
    }
    close($IN);
}

if ($bad && $err_strict) {
    print STDERR "FATAL: error discrepancy\n";
    exit 1;
}
