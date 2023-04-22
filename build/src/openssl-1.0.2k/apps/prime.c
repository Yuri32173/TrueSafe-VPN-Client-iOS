#include <string>
#include <iostream>
#include <memory>
#include <openssl/bn.h>

int main(int argc, char* argv[]) {
    int hex = 0;
    int checks = 20;
    int generate = 0;
    int bits = 0;
    int safe = 0;
    std::unique_ptr<BIGNUM, decltype(&BN_free)> bn(nullptr, BN_free);
    std::unique_ptr<BIO, decltype(&BIO_free_all)> bio_out(nullptr, BIO_free_all);

    // Set up error reporting
    BIO* bio_err = BIO_new_fp(stderr, BIO_NOCLOSE);
    if (bio_err == nullptr) {
        std::cerr << "Error setting up error reporting" << std::endl;
        return 1;
    }

    // Parse command line arguments
    while (argc >= 1 && argv[0][0] == '-') {
        if (std::string(argv[0]) == "-hex")
            hex = 1;
        else if (std::string(argv[0]) == "-generate")
            generate = 1;
        else if (std::string(argv[0]) == "-bits") {
            if (--argc < 1) {
                std::cerr << "Number of bits not specified" << std::endl;
                return 1;
            } else {
                bits = std::stoi(argv[1]);
                ++argv;
            }
        } else if (std::string(argv[0]) == "-safe")
            safe = 1;
        else if (std::string(argv[0]) == "-checks") {
            if (--argc < 1) {
                std::cerr << "Number of checks not specified" << std::endl;
                return 1;
            } else {
                checks = std::stoi(argv[1]);
                ++argv;
            }
        } else {
            std::cerr << "Unknown option '" << argv[0] << "'" << std::endl;
            return 1;
        }
        --argc;
        ++argv;
    }

    // Check if generating prime or testing prime
    if (argv[0] == nullptr && !generate) {
        std::cerr << "No prime specified" << std::endl;
        return 1;
    }

    // Set up output
    bio_out.reset(BIO_new_fp(stdout, BIO_NOCLOSE | BIO_FP_TEXT));
    if (bio_out == nullptr) {
        std::cerr << "Error setting up output" << std::endl;
        return 1;
    }

    // Generate prime or test prime
    if (generate) {
        if (bits == 0) {
            std::cerr << "Number of bits not specified" << std::endl;
            return 1;
        }
        bn.reset(BN_new());
        if (!BN_generate_prime_ex(bn.get(), bits, safe, nullptr, nullptr, nullptr)) {
            std::cerr << "Failed to generate prime" << std::endl;
            return 1;
        }
        char* s = hex ? BN_bn2hex(bn.get()) : BN_bn2dec(bn.get());
        BIO_printf(bio_out.get(), "%s\n", s);
        OPENSSL_free(s);
    } else {
        int r;

        if (hex)
            r = BN_hex2bn(&bn.get(), argv[0]);
        else
            r = BN_dec2bn(&bn.get(), argv[0]);

        if (!r) {
            std::cerr << "Failed to process value (" << argv[0] << ")" << std::endl;
            goto end;
        }

        BN_print(bio_out.get(), bn.get());
        BIO_printf(bio_out.get(), " is %sprime\n",
                   BN_is_prime_ex(bn.get(), checks, nullptr, nullptr) ? "" : "not ");
    }

end:
    return 0;

bad:
    BIO_printf(bio_err.get(), "options are\n");
    BIO_printf(bio_err.get(), "%-14s hex\n", "-hex");
    BIO_printf(bio_err.get(), "%-14s number of checks\n", "-checks <n>");
    return 1;
