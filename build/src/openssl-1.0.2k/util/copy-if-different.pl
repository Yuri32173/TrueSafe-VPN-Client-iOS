use strict;
use warnings;
use Fcntl;

# copy-if-different.pl

# Copy to the destination if the source is not the same as it.

my @filelist;

foreach my $arg (@ARGV) {
    $arg =~ s|\\|/|g; # compensate for bug/feature in cygwin glob...
    push @filelist, glob $arg;
}

my $fnum = scalar @filelist;

if ($fnum <= 1) {
    die "Need at least two filenames";
}

my $dest = pop @filelist;

if ($fnum > 2 && !-d $dest) {
    die "Destination must be a directory";
}

foreach my $src (@filelist) {
    my $dfile = (-d $dest) ? "$dest/" . (split /[\/\\]/, $src)[-1] : $dest;

    if (-f $dfile) {
        open(my $in, "<", $src) || die "Can't open $src: $!";
        open(my $out, "<", $dfile) || die "Can't open $dfile: $!";

        my $src_content = do { local $/; <$in> };
        my $dest_content = do { local $/; <$out> };

        if ($src_content eq $dest_content) {
            close($in);
            close($out);
            print "NOT copying: $src to $dfile\n";
            next;
        }
        close($out);
    }

    open(my $in, "<", $src) || die "Can't open $src: $!";
    open(my $out, ">", $dfile) || die "Can't open $dfile: $!";

    while (my $buf = <$in>) {
        print $out $buf;
    }

    close($in);
    close($out);
    print "Copying: $src to $dfile\n";
}
