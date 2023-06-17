#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/bn.h>
#include <openssl/dsa.h>
#include <openssl/x509.h>
#include <openssl/pem.h>

static int dsa_callback(int p, int n, BN_GENCB *cb) {
    char c = '*';

    if (p == 0) c = '.';
    if (p == 1) c = '+';
    if (p == 2) c = '*';
    if (p == 3) c = '\n';
    BIO_write(cb->arg, &c, 1);
    BIO_flush(cb->arg);

    return 1;
}

int main(int argc, char **argv) {
    DSA *dsa = NULL;
    int ret = 1;
    BIO *in = NULL, *out = NULL;
    int informat = FORMAT_PEM, outformat = FORMAT_PEM, noout = 0;
    char *infile = NULL, *outfile = NULL;
    int need_rand = 0;
    int numbits = -1, num, genkey = 0;
    
    if (argc < 1) {
        printf("Usage: %s [options] [bits] <infile >outfile\n", argv[0]);
        return 1;
    }

    in = BIO_new(BIO_s_file());
    out = BIO_new(BIO_s_file());
    if (!in || !out) {
        perror("Error creating BIO");
        goto end;
    }

    BIO_set_fp(out, stdout, BIO_NOCLOSE | BIO_FP_TEXT);

    infile = NULL;
    outfile = NULL;
    while (--argc >= 1) {
        if (strcmp(*++argv, "-in") == 0) {
            if (--argc < 1) goto bad;
            infile = *++argv;
        } else if (strcmp(*argv, "-out") == 0) {
            if (--argc < 1) goto bad;
            outfile = *++argv;
        } else if (strcmp(*argv, "-noout") == 0) {
            noout = 1;
        } else if (sscanf(*argv, "%d", &num) == 1) {
            /* generate a key */
            numbits = num;
            need_rand = 1;
        } else {
            printf("Unknown option %s\n", *argv);
            goto bad;
        }
    }

    if (infile == NULL)
        BIO_set_fp(in, stdin, BIO_NOCLOSE);
    else {
        if (BIO_read_filename(in, infile) <= 0) {
            perror(infile);
            goto end;
        }
    }

    if (outfile != NULL) {
        if (BIO_write_filename(out, outfile) <= 0) {
            perror(outfile);
            goto end;
        }
    }

    ERR_load_crypto_strings();

    if (need_rand) {
                app_RAND_load_file(NULL, 0);
        if (!app_RAND_load_file(NULL, 1) && !app_RAND_write_file(NULL))
            goto end;
    }

    if (noout) outformat = FORMAT_ASN1;

    dsa = DSA_new();
    if (dsa == NULL) {
        perror("Error creating DSA structure");
        goto end;
    }

    if (numbits != -1) {
        if (!DSA_generate_parameters_ex(dsa, numbits, NULL, 0, NULL, NULL, dsa_callback)) {
            perror("Error generating DSA parameters");
            goto end;
        }
        genkey = 1;
    } else {
        if (!PEM_read_bio_DSAparams(in, &dsa, NULL, NULL)) {
            perror("Error reading DSA parameters");
            goto end;
        }
    }

    if (genkey) {
        if (!DSA_generate_key(dsa)) {
            perror("Error generating DSA key");
            goto end;
        }
    }

    if (outformat == FORMAT_ASN1) {
        if (!i2d_DSAparams_bio(out, dsa)) {
            perror("Error writing DSA parameters");
            goto end;
        }
    } else {
        if (!PEM_write_bio_DSAparams(out, dsa)) {
            perror("Error writing DSA parameters");
            goto end;
        }
    }

    if (genkey && !noout) {
        if (outformat == FORMAT_ASN1) {
            if (!i2d_DSAPrivateKey_bio(out, dsa)) {
                perror("Error writing DSA private key");
                goto end;
            }
        } else {
            if (!PEM_write_bio_DSAPrivateKey(out, dsa, NULL, NULL, 0, NULL, NULL)) {
                perror("Error writing DSA private key");
                goto end;
            }
        }
    }

    ret = 0;

end:
    if (ret != 0) ERR_print_errors_fp(stderr);
    if (dsa != NULL) DSA_free(dsa);
    if (in != NULL) BIO_free(in);
    if (out != NULL) BIO_free_all(out);

    return ret;
}

