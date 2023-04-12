#pragma once

#include <cstdlib>
#include <string>
#include <memory>

#if defined(_WIN32)
#include <Windows.h>
#endif

#include <openssl/ossl_typ.h>
#include <openssl/e_os2.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(OPENSSL_FIPS)
#define FIPS_RAND_SIZE_T size_t
#endif

struct RandMethod {
    using SeedFn = void (*)(const void*, int);
    using BytesFn = int (*)(unsigned char*, int);
    using CleanupFn = void (*)();
    using AddFn = void (*)(const void*, int, double);
    using PseudoRandFn = int (*)(unsigned char*, int);
    using StatusFn = int (*)();

    SeedFn seed;
    BytesFn bytes;
    CleanupFn cleanup;
    AddFn add;
    PseudoRandFn pseudorand;
    StatusFn status;
};

#ifdef BN_DEBUG
extern int rand_predictable;
#endif

int setRandMethod(const RandMethod* method);
const RandMethod* getRandMethod();
#ifndef OPENSSL_NO_ENGINE
int setRandEngine(ENGINE* engine);
#endif
RandMethod* getSSLeayRand();
void cleanupRand();
int generateRandBytes(unsigned char* buffer, int numBytes);
int generatePseudoRandBytes(unsigned char* buffer, int numBytes);
void seedRand(const void* buffer, int numBytes);
void addRand(const void* buffer, int numBytes, double entropy);
int loadRandFile(const std::string& filename, long maxBytes);
int writeRandFile(const std::string& filename);
std::string getRandFileName(size_t maxLen);
int getRandStatus();
int queryEgdRandBytes(const std::string& path, unsigned char* buffer, int numBytes);
int egdRand(const std::string& path);
int egdRandBytes(const std::string& path, int numBytes);
int pollRand();

#if defined(_WIN32) || defined(_WIN64)

void randScreen();
int randEvent(UINT, WPARAM, LPARAM);

#endif

#ifdef OPENSSL_FIPS
void setFipsDrbgType(int type, int flags);
int initFipsRand();
#endif

/* Error codes for the RAND functions. */
enum class RandError {
    DUAL_EC_DRBG_DISABLED = 104,
    ERROR_INITIALISING_DRBG = 102,
    ERROR_INSTANTIATING_DRBG = 103,
    NO_FIPS_RANDOM_METHOD_SET = 101,
    PRNG_NOT_SEEDED = 100
};

void loadRandErrors();

#ifdef __cplusplus
}
#endif
