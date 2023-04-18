#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <openssl/bn.h>
#include <openssl/bio.h>

static const int DEFAULT_NUM_CHECKS = 20;

bool generate_prime(int bits, bool safe, char **result, size_t *result_len)
{
    BIGNUM *bn = BN_new();
    if (bn == NULL) {
        return false;
    }

    if (!BN_generate_prime_ex(bn, bits, safe, NULL, NULL, NULL)) {
        BN_free(bn);
        return false;
    }

    char *s = BN_bn2hex(bn);
    BN_free(bn);
    if (s == NULL) {
        return false;
    }

    *result_len = strlen(s);
    *result = malloc(*result_len + 1);
    if (*result == NULL) {
        OPENSSL_free(s);
        return false;
    }

    strcpy(*result, s);
    OPENSSL_free(s);
    return true;
}

bool check_prime(const char *prime, bool hex, int num_checks, bool *is_prime)
{
    BIGNUM *bn = NULL;
    bool success = false;

    if (hex) {
        BN_hex2bn(&bn, prime);
    } else {
        BN_dec2bn(&bn, prime);
    }

    if (bn == NULL) {
        goto cleanup;
    }

    if (BN_is_prime_ex(bn, num_checks, NULL, NULL)) {
        *is_prime = true;
    } else {
        *is_prime = false;
    }

    success = true;

cleanup:
    BN_free(bn);
    return success;
}

int main(int argc, char **argv) {
    bool hex = false;
    int checks = 20;
    bool generate = false;
    int bits = 0;
    bool safe = false;
    BIGNUM *bn = nullptr;
    auto bio_out = BIO_new_fp(stdout, BIO_NOCLOSE);

    while (argc >= 1 && argv[0][0] == '-') {
        auto arg = std::string(argv[0]);

        if (arg == "-hex") {
            hex = true;
        }
        else if (arg == "-generate") {
            generate = true;
        }
        else if (arg == "-bits") {
            if (--argc < 1) {
                std::cerr << "Error: -bits option requires an argument" << std::endl;
                return EXIT_FAILURE;
            }
            bits = std::atoi(*++argv);
        }
        else if (arg == "-safe") {
            safe = true;
        }
        else if (arg == "-checks") {
            if (--argc < 1) {
                std::cerr << "Error: -checks option requires an argument" << std::endl;
                return EXIT_FAILURE;
            }
            checks = std::atoi(*++argv);
        }
        else {
            std::cerr << "Error: unknown option '" << arg << "'" << std::endl;
            return EXIT_FAILURE;
        }

        --argc;
        ++argv;
    }

    if (argv[0] == nullptr && !generate) {
        std::cerr << "Error: no prime specified" << std::endl;
        return EXIT_FAILURE;
    }

    if (generate) {
        if (bits == 0) {
            std::cerr << "Error: -generate option requires the -bits option" << std::endl;
            return EXIT_FAILURE;
        }

        bn = BN_new();
        BN_generate_prime_ex(bn, bits, safe, nullptr, nullptr, nullptr);
        auto s = hex ? BN_bn2hex(bn) : BN_bn2dec(bn);
        BIO_printf(bio_out, "%s\n", s);
        OPENSSL_free(s);
    }
    else {
        if (hex) {
            BN_hex2bn(&bn, argv[0]);
        }
        else {
            BN_dec2bn(&bn, argv[0]);
        }

        BN_print(bio_out, bn);
        BIO_printf(bio_out, " is %sprime\n", BN_is_prime_ex(bn, checks, nullptr, nullptr) ? "" : "not ");
    }

    BN_free(bn);
    BIO_free_all(bio_out);

    return EXIT_SUCCESS;
}
