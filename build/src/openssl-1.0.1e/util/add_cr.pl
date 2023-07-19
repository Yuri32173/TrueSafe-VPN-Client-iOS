#!/usr/local/bin/perl
#
# This adds a copyright message to a source code file.
# It also gets the file name correct.
#
# perl util/add_cr.pl *.[ch] */*.[ch] */*/*.[ch]
#

foreach my $file (@ARGV) {
    dofile($file);
}

sub dofile {
    my ($file) = @_;

    open(my $IN, "<", $file) || die "unable to open $file: $!\n";

    print STDERR "doing $file\n";
    my @in = <$IN>;

    return 1 if ($in[0] =~ / NOCW /);

    my @out = ();
    open(my $OUT, ">", "$file.out") || die "unable to open $file.$$: $!\n";
    push(@out, "/* $file */\n");
    if (($in[1] !~ /^\/\* Copyright \(C\) [0-9-]+ Eric Young \(eay\@cryptsoft.com\)/)) {
        push(@out, &Copyright);
        my $i = 2;
        my @a = grep(/ Copyright \(C\) /, @in);
        if ($#a >= 0) {
            while (($i <= $#in) && ($in[$i] ne " */\n")) {
                $i++;
            }
            $i++ if ($in[$i] eq " */\n");

            while (($i <= $#in) && ($in[$i] =~ /^\s*$/)) {
                $i++;
            }

            push(@out, "\n");
            for (; $i <= $#in; $i++) {
                push(@out, $in[$i]);
            }
        } else {
            push(@out, @in);
        }
    } else {
        shift(@in);
        push(@out, @in);
    }
    print $OUT @out;
    close($IN);
    close($OUT);
    rename("$file", "$file.orig") || die "unable to rename $file: $!\n";
    rename("$file.out", $file) || die "unable to rename $file.out: $!\n";
}
