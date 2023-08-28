#!/usr/bin/env perl
use strict;
use warnings;
use File::Find;

die "usage: perlpath newpath  (eg /usr/bin)\n" unless @ARGV == 1;

find(\&wanted, ".");

sub wanted {
    return unless /\.pl$/i || /^[Cc]onfigur/i;

    open(my $IN, "<", $_) || die "unable to open $_: $!\n";
    my @lines = <$IN>;
    close($IN);

    my $new_first_line = "#!$ARGV[0]/perl\n";
    $new_first_line = "#!$ARGV[0]\n" unless -d $ARGV[0];

    my $new_file = "$_.new";
    open(my $OUT, ">", $new_file) || die "unable to open $new_file: $!\n";
    print $OUT $new_first_line, @lines[1..$#lines];
    close($OUT);

    rename($new_file, $_) || die "unable to rename $new_file: $!\n";
    chmod(0755, $_) || die "unable to chmod $_: $!\n";
}
