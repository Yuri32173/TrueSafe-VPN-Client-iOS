#pragma once

#include <cstddef>
#include <string_view>
#include <vector>
#include <openssl/opensslconf.h>

#ifdef OPENSSL_NO_AES
#error AES is disabled.
#endif

namespace openssl
{
    enum class aes_direction
    {
        encrypt = AES_ENCRYPT,
        decrypt = AES_DECRYPT
    };

    constexpr size_t aes_block_size = AES_BLOCK_SIZE;

    class aes_key
    {
    public:
        explicit aes_key(const std::vector<unsigned char>& user_key, int bits);

        int rounds() const noexcept { return rounds_; }
        const std::vector<unsigned char>& round_keys() const noexcept { return round_keys_; }

    private:
        std::vector<unsigned char> round_keys_;
        int rounds_;
    };

    class aes_cipher
    {
    public:
        explicit aes_cipher(const aes_key& key, const unsigned char* iv = nullptr);

        std::vector<unsigned char> encrypt(std::string_view plaintext);
        std::vector<unsigned char> decrypt(std::string_view ciphertext);

    private:
        AES_KEY key_;
        std::vector<unsigned char> iv_;
    };
}
