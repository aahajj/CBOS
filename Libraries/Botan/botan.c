#include "botan.h"

/**
 * @return message size.
 */
int get_message_size()
{
	return MESSAGE_SIZE;
}

/**
 * @return number of iterations for benchmarking.
 */
int get_iterations()
{
	return ITERATIONS;
}

/**
 * @return "Botan (Version number)" as a char pointer.
 */
const char *botan_name()
{
	const char *full_version_string = botan_version_string();
	static char short_version_string[12];

	// Copy the name and version number to the short new String
	strncpy(short_version_string, full_version_string, sizeof(short_version_string) - 1);

	return short_version_string;
}

/**
 * Get a list of ciphers supported by the Botan library.
 * @return An array of cipher names as strings, with a NULL-terminated
 * sentinel.
 */
const char **botan_ciphers()
{
    static const char *names[] = {
#if defined(AES_128)
    #if defined(ECB)
        CIPHER_AES_128_ECB,
    #else
        CIPHER_AES_128_CTR,
        CIPHER_AES_128_CBC,
        CIPHER_AES_128_CFB,
        CIPHER_AES_128_GCM,
        CIPHER_AES_128_OCB,
        CIPHER_AES_128_CCM,
    #endif
#elif defined(AES_192)
    #if defined(ECB)
        CIPHER_AES_192_ECB,
    #else    
        CIPHER_AES_192_CTR,
        CIPHER_AES_192_CBC,
        CIPHER_AES_192_CFB,
        CIPHER_AES_192_GCM,
        CIPHER_AES_192_OCB,
        CIPHER_AES_192_CCM,
    #endif
#elif defined(AES_256)
    #if defined(ECB)
        CIPHER_AES_256_ECB,
    #else
        CIPHER_AES_256_CTR,
        CIPHER_AES_256_CBC,
        CIPHER_AES_256_CFB,
        CIPHER_AES_256_GCM,
        CIPHER_AES_256_OCB,
        CIPHER_AES_256_CCM,
    #endif
#endif
        NULL
    };
    return names;
}

/**
 * Generate random data.
 * @param param A pointer to the cryptographic context.
 * @param size The size of random data to generate.
 * @param data A pointer to the destination buffer for the random data.
 * @return True if random data is generated successfully, otherwise false.
 */
bool botan_random(void *param, const size_t size, void *data)
{
	if (!param || !data)
	{
		return false;
	}

	random_bytes(data, size);

	if (!data)
		return false;
	return true;
}

/**
 * Free resources associated with BotanParam.
 * @param param A pointer to the context to be freed.
 * @return True if resources are successfully freed, otherwise false.
 */
bool botan_free(void *param)
{
	if (!param)
	{
		return false;
	}

	BotanParam *op = param;

	#if defined(ECB)
		op->error = botan_block_cipher_destroy(op->bc);
	else
		op->error = botan_cipher_destroy(op->cipher);
	#endif

	free(op);

	return true;
}

/**
 * BotanParam Initialization
 * @param param A pointer to a void pointer where BotanParam will be stored.
 * @return True if initialization is successful, otherwise false.
 */
bool botan_init(void **param)
{
	if (!param)
	{
		return false;
	}

	BotanParam *op = malloc(sizeof(BotanParam));
	*param = op;

	op->error = false;
	op->output_written = 0;
	op->input_consumed = 0;

	if (!op)
	{
		printf("Error : botan_init() : BotanParam Initialization has failed !\n");
		return !op->error;
	}

	return !op->error;
}

/**
 * Set the cipher and keys to be used for cryptographic operations.
 * @param param A pointer to the cryptographic context.
 * @param cipher The name of the cipher as a null-terminated string.
 * @return True if the cipher is successfully set, otherwise false.
 */
bool botan_set_cipher(void *param, const char *cipher)
{
	if (!param || !cipher)
	{
		return false;
	}

	BotanParam *op = param;

	#if defined(ECB)
		handle_botan_block_cipher(&op->error, param, cipher);	
	#else
		handle_botan_cipher(&op->error, param, cipher);
	#endif

	return !op->error;
}

/**
 * Encrypt data.
 * @param param A pointer to the cryptographic context.
 * @param size The size of the source data.
 * @param dst A pointer to the destination buffer for the encrypted data.
 * @param src A pointer to the source data to be encrypted.
 * @return zero on success or an error code.
 */
size_t botan_encrypt(void *param, const size_t size, void *dst, const void *src)
{
	BotanParam *op = param;

	#if defined(ECB)
		return !botan_block_cipher_encrypt_blocks(op->bc, (const uint8_t *)src, (uint8_t *)dst, (MESSAGE_SIZE / 16));
	#else			    
		return !botan_cipher_update(op->cipher, 0, dst, MESSAGE_SIZE, &op->output_written, src, MESSAGE_SIZE,
									  &op->input_consumed);
#endif
}

/**
 * Prepare Botan to be called by main by defining pointers to functions
 * containing the implementation.
 *
 * @return a pointer to a struct containing function pointers that implement
 * Botan.
 */
const Crypto *get_lib()
{
	static const Crypto crypto = {
		botan_name,
		botan_ciphers,
		botan_init,
		botan_free,
		botan_random,
		botan_set_cipher,
		botan_encrypt,
	};

	return &crypto;
}

#if defined(ECB)
/**
 * Initialize the block cipher.
 * @param error A pointer to a boolean that will be set to true if an error
 * occurs.
 * @param param A pointer to the cryptographic context.
 * @param cipher The name of the cipher as a null-terminated string
 * @return void
 * @note This function is called by botan_set_cipher() to initialize the
 * cipher. It is not intended to be called directly by main() or other
 * functions.
 */
void handle_botan_block_cipher(bool *error, void *param, const char *cipher)
{
	BotanParam *op = param;

	*error = botan_block_cipher_init(&op->bc, cipher);
	if (*error)
	{
		printf("Error: botan_block_cipher_init(): Block cipher Initialization has "
			   "failed!\n");
		return;
	}

	*error = botan_block_cipher_set_key(op->bc, op->key, KEY_SIZE);
	if (*error)
	{
		printf("Error setting key for %s\n!", cipher);
	};
}
#endif

/**
 * Initialize the cipher.
 * @param error A pointer to a boolean that will be set to true if an error
 * occurs.
 * @param param A pointer to the cryptographic context.
 * @param cipher The name of the cipher as a null-terminated string
 * @return void
 * @note This function is called by botan_set_cipher() to initialize the
 * cipher. It is not intended to be called directly by main() or other
 * functions.
 */
void handle_botan_cipher(bool *error, void *param, const char *cipher)
{
	BotanParam *op = param;

	/*
	The implementation of botan_cipher_is_authenticated() is missing.
	The following code is a workaround to check if the current cipher is
	authenticated or not. https://github.com/randombit/botan/issues/3904
	*/
	size_t is_authenticated = 0;
	botan_cipher_get_tag_length(op->cipher, &is_authenticated);

	// which IV size do I need?
	size_t iv_size = is_authenticated ? AEAD_IV_SIZE : IV_SIZE;

	*error = botan_cipher_init(&op->cipher, cipher, BOTAN_CIPHER_INIT_FLAG_ENCRYPT);
	if (*error)
	{
		printf("Error: botan_cipher_init(): Cipher Initialization has failed!\n");
		return;
	}

	if (!botan_random(param, KEY_SIZE, op->key) || !botan_random(param, iv_size, op->iv))
	{
		printf("Error: botan_random() has failed!\n");
		return;
	}

	*error = botan_cipher_set_key(op->cipher, op->key, KEY_SIZE);
	if (*error)
	{
		printf("Error setting key for %s!\n", cipher);
		return;
	}

	if (is_authenticated > 0)
	{
		*error = botan_cipher_set_associated_data(op->cipher, (const uint8_t *)"ADADADADADADADAD", 16);
		if (*error)
		{
			printf("Error: botan_cipher_set_associated_data(): setting "
				   "associated data has failed for %s!\n",
				   cipher);
			return;
		}
	}

	*error = botan_cipher_start(op->cipher, op->iv, iv_size);
	if (*error)
	{
		printf("Error: botan_cipher_start(): starting cipher has failed for %s!\n", cipher);
		return;
	}
}