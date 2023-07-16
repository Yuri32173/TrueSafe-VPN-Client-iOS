#!/usr/local/bin/perl

use strict;
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
    my $dfile;

    if (-d $dest) {
        $dfile = $src;
        $dfile =~ s|^.*[/\\]([^/\\]*)$|$1|;
        $dfile = "$dest/$dfile";
    } else {
        $dfile = $dest;
    }

    my $buf;
    if (-f $dfile) {
        sysopen(my $IN, $src, O_RDONLY | O_BINARY) || die "Can't Open $src";
        sysopen(my $OUT, $dfile, O_RDONLY | O_BINARY) || die "Can't Open $dfile";
        while (sysread $IN, $buf, 10240) {
            my $b2;
            goto copy if !sysread($OUT, $b2, 10240) || $buf ne $b2;
        }
        goto copy if sysread($OUT, $buf, 1);
        close($IN);
        close($OUT);
        print "NOT copying: $src to $dfile\n";
        next;
    }

  copy:
    sysopen(my $IN, $src, O_RDONLY | O_BINARY) || die "Can't Open $src";
    sysopen(my $OUT, $dfile, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY) || die "Can't Open $dfile";
    while (sysread $IN, $buf, 10240) {
        syswrite($OUT, $buf, length($buf));
    }
    close($IN);
    close($OUT);
    print "Copying: $src to $dfile\n";
}
