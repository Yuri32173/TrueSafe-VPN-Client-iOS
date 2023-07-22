#!/usr/bin/perl
#
# CA - wrapper around ca to make it easier to use ... basically ca requires
#      some setup stuff to be done before you can use it and this makes
#      things easier between now and when Eric is convinced to fix it :-)
#
# CA -newca ... will setup the right stuff
# CA -newreq[-nodes] ... will generate a certificate request 
# CA -sign ... will sign the generated request and output 
#
# At the end of that grab newreq.pem and newcert.pem (one has the key 
# and the other the certificate) and cat them together and that is what
# you want/need ... I'll make even this a little cleaner later.
#
#
# 12-Jan-96 tjh    Added more things ... including CA -signcert which
#                  converts a certificate to a request and then signs it.
# 10-Jan-96 eay    Fixed a few more bugs and added the SSLEAY_CONFIG
#		   environment variable so this can be driven from
#		   a script.
# 25-Jul-96 eay    Cleaned up filenames some more.
# 11-Jun-96 eay    Fixed a few filename missmatches.
# 03-May-96 eay    Modified to use 'ssleay cmd' instead of 'cmd'.
# 18-Apr-96 tjh    Original hacking
#
# Tim Hudson
# tjh@cryptsoft.com
#

# 27-Apr-98 snh    Translation into perl, fix existing CA bug.
#
#
# Steve Henson
# shenson@bigfoot.com

# default openssl.cnf file has setup as per the following
# demoCA ... where everything is stored

#!/usr/bin/perl
use strict;
use warnings;
use Getopt::Long;

my $openssl = $ENV{OPENSSL} || "openssl";
$ENV{OPENSSL} = $openssl;

my $SSLEAY_CONFIG = $ENV{SSLEAY_CONFIG};
my $DAYS = "-days 365";    # 1 year
my $CADAYS = "-days 1095"; # 3 years
my $REQ = "$openssl req $SSLEAY_CONFIG";
my $CA = "$openssl ca $SSLEAY_CONFIG";
my $VERIFY = "$openssl verify";
my $X509 = "$openssl x509";
my $PKCS12 = "$openssl pkcs12";

my $CATOP = "./demoCA";
my $CAKEY = "cakey.pem";
my $CAREQ = "careq.pem";
my $CACERT = "cacert.pem";

my $DIRMODE = 0777;
my $RET = 0;

sub cp_pem {
    my ($infile, $outfile, $bound) = @_;
    open my $in, '<', $infile;
    open my $out, '>', $outfile;
    my $flag = 0;
    while (<$in>) {
        $flag = 1 if (/^-----BEGIN.*$bound/);
        print $out $_ if ($flag);
        if (/^-----END.*$bound/) {
            close $in;
            close $out;
            return;
        }
    }
}

my %opts;
GetOptions(\%opts,
    "help|h"
) or die "Usage: $0 -newcert|-newreq|-newreq-nodes|-newca|-sign|-verify\n";

if ($opts{help}) {
    print STDERR "Usage: $0 -newcert|-newreq|-newreq-nodes|-newca|-sign|-verify\n";
    exit 0;
}

foreach (@ARGV) {
    if (/^-newcert$/) {
        # create a certificate
        system("$REQ -new -x509 -keyout newkey.pem -out newcert.pem $DAYS");
        $RET = $?;

        print "Certificate is in newcert.pem, private key is in newkey.pem\n";
    } elsif (/^-newreq$/) {
        # create a certificate request
        system("$REQ -new -keyout newkey.pem -out newreq.pem $DAYS");
        $RET = $?;

        print "Request is in newreq.pem, private key is in newkey.pem\n";
    } elsif (/^-newreq-nodes$/) {
        # create a certificate request without encryption
        system("$REQ -new -nodes -keyout newkey.pem -out newreq.pem $DAYS");
        $RET = $?;

        print "Request is in newreq.pem, private key is in newkey.pem\n";
    } elsif (/^-newca$/) {
        # Set up the directory structure if it doesn't exist
        my $NEW = 1;
        if ($NEW || ! -f "${CATOP}/serial") {
            # Create the directory hierarchy
            mkdir $CATOP, $DIRMODE;
            mkdir "${CATOP}/certs", $DIRMODE;
            mkdir "${CATOP}/crl", $DIRMODE;
            mkdir "${CATOP}/newcerts", $DIRMODE;
            mkdir "${CATOP}/private", $DIRMODE;
            open my $out, '>', "${CATOP}/index.txt";
            close $out;
            open $out, '>', "${CATOP}/crlnumber";
            print $out "01\n";
            close $out;
        }
        if (! -f "${CATOP}/private/$CAKEY") {
            print "CA certificate filename (or enter to create)\n";
            my $FILE = <STDIN>;

            chomp $FILE;

            if ($FILE) {
                cp_pem($FILE, "${CATOP}/private/$CAKEY", "PRIVATE");
                cp_pem($FILE, "${CATOP}/$CACERT", "CERTIFICATE");
                $RET = $?;
            } else {
                print "Making CA certificate ...\n";
                system("$REQ -new -keyout " .
                    "${CATOP}/private/$CAKEY -out ${CATOP}/$CAREQ");
                system("$CA -create_serial " .
                    "-out ${CATOP}/$CACERT $CADAYS -batch " . 
                    "-keyfile ${CATOP}/private/$CAKEY -selfsign " .
                    "-extensions v3_ca " .
                    "-infiles ${CATOP}/$CAREQ ");
                $RET = $?;
            }
        }
    } elsif (/^-pkcs12$/) {
        my $cname = $ARGV[1] // "My Certificate";
        system("$PKCS12 -in newcert.pem -inkey newkey.pem " .
            "-certfile ${CATOP}/$CACERT -out newcert.p12 " .
            "-export -name \"$cname\"");
        $RET = $?;

        print "PKCS #12 file is in newcert.p12\n";
        exit $RET;
    } elsif (/^-xsign$/) {
        system("$CA -policy policy_anything -infiles newreq.pem");
        $RET = $?;
    } elsif (/^(-sign|-signreq)$/) {
        system("$CA -policy policy_anything -out newcert.pem " .
            "-infiles newreq.pem");
        $RET = $?;
        print "Signed certificate is in newcert.pem\n";
    } elsif (/^(-signCA)$/) {
        system("$CA -policy policy_anything -out newcert.pem " .
            "-extensions v3_ca -infiles newreq.pem");
        $RET = $?;
        print "Signed CA certificate is in newcert.pem\n";
    } elsif (/^-signcert$/) {
        system("$X509 -x509toreq -in newreq.pem -signkey newreq.pem " .
            "-out tmp.pem");
        system("$CA -policy policy_anything -out newcert.pem " .
            "-infiles tmp.pem");
        $RET = $?;
        print "Signed certificate is in newcert.pem\n";
    } elsif (/^-verify$/) {
        if (shift) {
            foreach my $file (@ARGV) {
                system("$VERIFY -CAfile $CATOP/$CACERT $file");
                $RET = $? if ($? != 0);
            }
            exit $RET;
        } else {
            system("$VERIFY -CAfile $CATOP/$CACERT newcert.pem");
            $RET = $?;
            exit 0;
        }
    } else {
        print STDERR "Unknown arg $_\n";
        print STDERR "Usage: $0 -newcert|-newreq|-newreq-nodes|-newca|-sign|-verify\n";
        exit 1;
    }
}

exit $RET;
