#pragma once

#define AES_128
#define IV_SIZE 16
#define AEAD_IV_SIZE 12

#ifdef AES_128
    #define KEY_SIZE 16
#elif defined(AES_192)
    #define KEY_SIZE 24
#elif defined(AES_256)
    #define KEY_SIZE 32
#endif

#if defined(AES_128)
    #if defined(ECB)
        #define CIPHER_AES_128_ECB "AES-128"
    #else
        #define CIPHER_AES_128_CBC "AES-128/CBC"
        #define CIPHER_AES_128_CTR "AES-128/CTR"
        #define CIPHER_AES_128_CFB "AES-128/CFB"
        #define CIPHER_AES_128_GCM "AES-128/GCM"
        #define CIPHER_AES_128_OCB "AES-128/OCB"
        #define CIPHER_AES_128_CCM "AES-128/CCM"
    #endif
#elif defined(AES_192)
    #if defined(ECB)
        #define CIPHER_AES_192_ECB "AES-192"
    #else
        #define CIPHER_AES_192_CBC "AES-192/CBC"
        #define CIPHER_AES_192_CTR "AES-192/CTR"
        #define CIPHER_AES_192_CFB "AES-192/CFB"
        #define CIPHER_AES_192_GCM "AES-192/GCM"
        #define CIPHER_AES_192_OCB "AES-192/OCB"
        #define CIPHER_AES_192_CCM "AES-192/CCM"
    #endif
#elif defined(AES_256)
    #if defined(ECB)
        #define CIPHER_AES_256_ECB "AES-256"
    #else
        #define CIPHER_AES_256_CBC "AES-256/CBC"
        #define CIPHER_AES_256_CTR "AES-256/CTR"
        #define CIPHER_AES_256_CFB "AES-256/CFB"
        #define CIPHER_AES_256_GCM "AES-256/GCM"
        #define CIPHER_AES_256_OCB "AES-256/OCB"
        #define CIPHER_AES_256_CCM "AES-256/CCM"
    #endif
#endif

#ifndef MESSAGE_SIZE
    #define MESSAGE_SIZE 4096
#endif

#ifndef ITERATIONS
    #define ITERATIONS 100
#endif

#include "../../src/cbos.h"
#include "botan/ffi.h"

typedef struct BotanParam
{
    #if defined(ECB)
        botan_block_cipher_t bc; 
    #else
        botan_cipher_t cipher;
    #endif
        bool error;
        unsigned char key[KEY_SIZE];
        unsigned char iv[IV_SIZE];
        size_t output_written;
        size_t input_consumed;
} BotanParam;

/* helper functions that are used by botan_set_cipher() */
void handle_botan_block_cipher(bool *error, void *param, const char *cipher);
void handle_botan_cipher(bool *error, void *param, const char *cipher);