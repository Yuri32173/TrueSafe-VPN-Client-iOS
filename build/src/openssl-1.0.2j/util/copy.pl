#!/usr/local/bin/perl

use strict;
use warnings;
use Fcntl;

# copy.pl

# Perl script 'copy' comment. On Windows the built-in "copy" command also
# copies timestamps: this messes up Makefile dependencies.

my $stripcr = 0;

foreach my $arg (@ARGV) {
    if ($arg eq "-stripcr") {
        $stripcr = 1;
        next;
    }
    $arg =~ s|\\|/|g;    # compensate for bug/feature in cygwin glob...
    foreach (glob $arg) {
        push @filelist, $_;
    }
}

my $fnum = @filelist;

if ($fnum <= 1) {
    die "Need at least two filenames";
}

my $dest = pop @filelist;

if ($fnum > 2 && ! -d $dest) {
    die "Destination must be a directory";
}

foreach my $file (@filelist) {
    my $dfile;
    if (-d $dest) {
        $dfile = $file;
        $dfile =~ s|^.*[/\\]([^/\\]*)$|$1|;
        $dfile = "$dest/$dfile";
    } else {
        $dfile = $dest;
    }
    sysopen(my $IN, $file, O_RDONLY|O_BINARY) || die "Can't Open $file";
    sysopen(my $OUT, $dfile, O_WRONLY|O_CREAT|O_TRUNC|O_BINARY)
        || die "Can't Open $dfile";
    while (sysread $IN, my $buf, 10240) {
        if ($stripcr) {
            $buf =~ tr/\015//d;
        }
        syswrite($OUT, $buf, length($buf));
    }
    close($IN);
    close($OUT);
    print "Copying: $file to $dfile\n";
}
