#ifndef HEADER_AES_H
#define HEADER_AES_H

#include <openssl/opensslconf.h>

#ifdef OPENSSL_NO_AES
#error AES is disabled.
#endif

#include <cstddef>

constexpr int AES_ENCRYPT = 1;
constexpr int AES_DECRYPT = 0;

constexpr int AES_MAXNR = 14;
constexpr int AES_BLOCK_SIZE = 16;

#ifdef __cplusplus
extern "C" {
#endif

struct AES_KEY {
#ifdef AES_LONG
    unsigned long rd_key[4 * (AES_MAXNR + 1)];
#else
    unsigned int rd_key[4 * (AES_MAXNR + 1)];
#endif
    int rounds;
};

const char* AES_options(void);

int AES_set_encrypt_key(const unsigned char* userKey, const int bits, AES_KEY* key);
int AES_set_decrypt_key(const unsigned char* userKey, const int bits, AES_KEY* key);

int private_AES_set_encrypt_key(const unsigned char* userKey, const int bits, AES_KEY* key);
int private_AES_set_decrypt_key(const unsigned char* userKey, const int bits, AES_KEY* key);

void AES_encrypt(const unsigned char* in, unsigned char* out, const AES_KEY* key);
void AES_decrypt(const unsigned char* in, unsigned char* out, const AES_KEY* key);

void AES_ecb_encrypt(const unsigned char* in, unsigned char* out, const AES_KEY* key, const int enc);
void AES_cbc_encrypt(const unsigned char* in, unsigned char* out, size_t length, const AES_KEY* key,
                     unsigned char* ivec, const int enc);
void AES_cfb128_encrypt(const unsigned char* in, unsigned char* out, size_t length, const AES_KEY* key,
                        unsigned char* ivec, int* num, const int enc);
void AES_cfb1_encrypt(const unsigned char* in, unsigned char* out, size_t length, const AES_KEY* key,
                      unsigned char* ivec, int* num, const int enc);
void AES_cfb8_encrypt(const unsigned char* in, unsigned char* out, size_t length, const AES_KEY* key,
                      unsigned char* ivec, int* num, const int enc);
void AES_ofb128_encrypt(const unsigned char* in, unsigned char* out, size_t length, const AES_KEY* key,
                        unsigned char* ivec, int* num);
void AES_ctr128_encrypt(const unsigned char* in, unsigned char* out, size_t length, const AES_KEY* key,
                        unsigned char ivec[AES_BLOCK_SIZE], unsigned char ecount_buf[AES_BLOCK_SIZE],
                        unsigned int* num);
void AES_ige_encrypt(const unsigned char* in, unsigned char* out, size_t length, const AES_KEY* key,
                     unsigned char* ivec, const int enc);
void AES_bi_ige_encrypt(const unsigned char* in, unsigned char* out, size_t length, const AES_KEY* key,
                        const AES_KEY* key2, const unsigned char* ivec, const int enc);

int AES_wrap_key(AES_KEY* key, const unsigned char* iv, unsigned char* out,
                 const unsigned char* in, unsigned int inlen);
int AES_unwrap_key(AES_KEY* key, const unsigned char* iv, unsigned char* out,
                   const unsigned char* in, unsigned int inlen);

#ifdef __cplusplus
}
#endif

#endif  // HEADER_AES_H
