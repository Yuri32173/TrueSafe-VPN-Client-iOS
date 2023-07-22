#!/usr/local/bin/perl

#!/usr/bin/perl
use strict;
use warnings;
use File::Copy;

my $stripcr = 0;
my @filelist;

foreach my $arg (@ARGV) {
	if ($arg eq "-stripcr") {
		$stripcr = 1;
		next;
	}
	$arg =~ s|\\|/|g;	# compensate for bug/feature in cygwin glob...
	push @filelist, glob $arg;
}

my $fnum = @filelist;

if ($fnum < 2) {
	die "Need at least two filenames";
}

my $dest = pop @filelist;

if ($fnum > 2 && !-d $dest) {
	die "Destination must be a directory";
}

foreach my $source (@filelist) {
	my $destination;
	if (-d $dest) {
		my ($filename) = $source =~ m{([^/\\]+)$};
		$destination = "$dest/$filename";
	} else {
		$destination = $dest;
	}

	if ($stripcr) {
		open my $in_fh, '<:raw', $source or die "Can't open $source: $!";
		open my $out_fh, '>:raw', $destination or die "Can't open $destination: $!";
		while (read $in_fh, my $buf, 10240) {
			$buf =~ tr/\015//d;
			print $out_fh $buf;
		}
		close $in_fh;
		close $out_fh;
	} else {
		copy($source, $destination) or die "Copy failed: $!";
	}

	print "Copying: $source to $destination\n";
}

