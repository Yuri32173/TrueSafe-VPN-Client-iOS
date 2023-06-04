#include <stdio.h>
#include <string.h>
#include <openssl/des.h>
#include <openssl/rc4.h>

void rc4()
{
    static const char *keystr = "0123456789abcdef";
    RC4_KEY key;
    unsigned char in[100], out[100];
    int i;

    RC4_set_key(&key, 16, (const unsigned char *)keystr);

    in[0] = '\0';
    strcpy((char *)in, "0123456789ABCEDFdata 12345");
    RC4(&key, 26, in, out);

    for (i = 0; i < 26; i++)
        fprintf(stderr, "%02X ", out[i]);
    fprintf(stderr, "\n");

    in[0] = '\0';
    strcpy((char *)in, "9876543210abcdefdata 12345");
    RC4(&key, 26, in, out);

    for (i = 0; i < 26; i++)
        fprintf(stderr, "%02X ", out[i]);
    fprintf(stderr, "\n");
}

void des()
{
    DES_key_schedule ks;
    DES_cblock iv, key;
    int num;
    static const char *keystr = "01234567";
    static const char *in1 = "0123456789ABCEDFdata 12345";
    static const char *in2 = "9876543210abcdefdata 12345";
    unsigned char out[100];
    int i;

    DES_set_key((const_DES_cblock *)keystr, &ks);

    num = 0;
    memset(iv, 0, 8);
    DES_cfb64_encrypt((const unsigned char *)in1, out, 26, &ks, &iv, &num, DES_ENCRYPT);
    for (i = 0; i < 26; i++)
        fprintf(stderr, "%02X ", out[i]);
    fprintf(stderr, "\n");

    num = 0;
    memset(iv, 0, 8);
    DES_cfb64_encrypt((const unsigned char *)in2, out, 26, &ks, &iv, &num, DES_ENCRYPT);
    for (i = 0; i < 26; i++)
        fprintf(stderr, "%02X ", out[i]);
    fprintf(stderr, "\n");
}

int main()
{
    fprintf(stderr, "rc4\n");
    rc4();
    fprintf(stderr, "cfb des\n");
    des();

    return 0;
}
