use strict;
use warnings;

my $err_strict = 0;
my $bad = 0;

foreach my $file (@ARGV) {
    if ($file eq "-strict") {
        $err_strict = 1;
        next;
    }

    open(my $in, "<", $file) || die "Unable to open $file: $!\n";
    my $func = "";
    while (my $line = <$in>) {
        if (!/;$/ && /^\**([a-zA-Z].*[\s*])?([A-Za-z_0-9]+)\(.*([),]|$)/) {
            /^([^()]*(\([^()]*\)[^()]*)*)\(/;
            my $match = $1;
            $match =~ /([A-Za-z_0-9]*)$/;
            $func = lc($1);
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
                # print "check -$file:$.:$func:$n\n";
                next;
            }

            my $lib = $1;
            $n = lc($2);

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
            # print "$func:$1\n";
        }
    }
    close($in);
}

if ($bad && $err_strict) {
    print STDERR "FATAL: error discrepancy\n";
    exit 1;
}

