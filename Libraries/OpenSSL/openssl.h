#pragma once

#define AES_128
#define IV_SIZE 16

#ifdef AES_128
#define KEY_SIZE 16
#endif

#ifdef AES_192
#define KEY_SIZE 24
#endif

#ifdef AES_256
#define KEY_SIZE 32
#endif

#if KEY_SIZE == 16
#define CIPHER_AES_128_ECB "AES-128-ECB"
#define CIPHER_AES_128_CBC "AES-128-CBC"
#define CIPHER_AES_128_CTR "AES-128-CTR"
#define CIPHER_AES_128_CFB "AES-128-CFB"
#elif KEY_SIZE == 24
#define CIPHER_AES_192_ECB "AES-192-ECB"
#define CIPHER_AES_192_CBC "AES-192-CBC"
#define CIPHER_AES_192_CTR "AES-192-CTR"
#define CIPHER_AES_192_CFB "AES-192-CFB"
#else
#define CIPHER_AES_256_ECB "AES-256-ECB"
#define CIPHER_AES_256_CBC "AES-256-CBC"
#define CIPHER_AES_256_CTR "AES-256-CTR"
#define CIPHER_AES_192_CFB "AES-256-CFB"
#endif

#ifndef MESSAGE_SIZE
#define MESSAGE_SIZE 4096
#endif

#ifndef ITERATIONS
#define ITERATIONS 100
#endif

#include "../../src/cbos.h"
#include "openssl/err.h"
#include "openssl/evp.h"
#include "openssl/opensslv.h"

typedef struct OpenSSLParam
{
	unsigned char key[KEY_SIZE];
	unsigned char iv[IV_SIZE];
	EVP_CIPHER_CTX *ctx_encrypt;
	const EVP_CIPHER *current_cipher;
} OpenSSLParam;
