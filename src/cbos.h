#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

/**
 * @struct Crypto
 * @brief This struct defines function pointers that can be used to interact
 * with cryptographic libraries.
 */
typedef struct Crypto
{
	const char *(*name)();		// Function to get library name
	const char **(*ciphers)();	// Function to get supported ciphers
	bool (*init)(void **param); // Function to initialize cipher parameters
	bool (*free)(void *param);
	bool (*random)(void *param, const size_t size, void *dst);
	bool (*set_cipher)(void *param, const char *cipher);
	size_t (*encrypt)(void *param, const size_t size, void *dst, const void *src);
} Crypto;

/**
 * @brief Function to get the implementation of the current cryptographic library.
 *
 * @return A pointer to the current cryptographic library (Crypto struct).
 */
const Crypto *get_lib();

/**
 * @brief Gets the message size for benchmarking defined by user.
 *
 * @return The message size.
 */
int get_message_size();

/**
 * @brief Gets the number of iterations defined by user.
 *
 * @return The number of iterations.
 */
int get_iterations();

/**
 * @brief Function to generate random bytes.
 *
 * @param data Pointer to the destination buffer.
 * @param size Number of random bytes to generate.
 */
void random_bytes(uint8_t *data, size_t size);
