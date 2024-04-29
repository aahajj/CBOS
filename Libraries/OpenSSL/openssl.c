#include "openssl.h"

#define openssl_error() (ERR_error_string(ERR_get_error(), NULL))

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
 * Get the name of the OpenSSL cryptographic library.
 * @return The name of the OpenSSL library as a string.
 */
const char *openssl_name()
{
	return OPENSSL_VERSION_TEXT;
}

/**
 * Get a list of ciphers supported by the OpenSSL library.
 * @return An array of cipher names as strings, with a NULL-terminated sentinel.
 */
const char **openssl_ciphers()
{
	static const char *names[] = {
#if KEY_SIZE == 16
		CIPHER_AES_128_ECB,
		CIPHER_AES_128_CBC,
		CIPHER_AES_128_CTR,
		CIPHER_AES_128_CFB,
#elif KEY_SIZE == 24
		CIPHER_AES_192_ECB,
		CIPHER_AES_192_CBC,
		CIPHER_AES_192_CTR,
		CIPHER_AES_192_CFB,
#else
		CIPHER_AES_256_ECB,
		CIPHER_AES_256_CBC,
		CIPHER_AES_256_CTR,
		CIPHER_AES_192_CFB,
#endif
		NULL
	};

	return names;
}

/**
 * Initialize the OpenSSL cryptographic context and parameter.
 * @param param A pointer to a void pointer where the context will be stored.
 * @return True if initialization is successful, otherwise false.
 */
bool openssl_init(void **param)
{
	if (!param)
	{
		return false;
	}

	OpenSSLParam *op = malloc(sizeof(OpenSSLParam));

	op->ctx_encrypt = EVP_CIPHER_CTX_new();

	*param = op;
	return true;
}

/**
 * Free resources associated with the OpenSSL context.
 * @param param A pointer to the context to be freed.
 * @return True if resources are successfully freed, otherwise false.
 */
bool openssl_free(void *param)
{
	if (!param)
	{
		return false;
	}

	OpenSSLParam *op = param;

	free(op);

	return true;
}

/**
 * Generate random data.
 * @param param A pointer to the cryptographic context.
 * @param size The size of random data to generate.
 * @param data A pointer to the destination buffer for the random data.
 * @return True if random data is generated successfully, otherwise false.
 */
bool openssl_random(void *param, const size_t size, void *data)
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
 * Set the cipher and keys to be used for cryptographic operations.
 * @param param A pointer to the cryptographic context.
 * @param cipher The name of the cipher as a null-terminated string.
 * @return True if the cipher is successfully set, otherwise false.
 */
bool openssl_set_cipher(void *param, const char *cipher)
{
	if (!param || !cipher)
	{
		return false;
	}

	OpenSSLParam *op = param;
	const char **ciphers = openssl_ciphers();

	if (strcmp(cipher, ciphers[0]) == 0)
	{
		op->current_cipher = EVP_aes_128_ecb();
	}

	else if (strcmp(cipher, ciphers[1]) == 0)
	{
		op->current_cipher = EVP_aes_128_cbc();
	}
	else if (strcmp(cipher, ciphers[2]) == 0)
	{
		op->current_cipher = EVP_aes_128_ctr();
	}
	else if (strcmp(cipher, ciphers[3]) == 0)
	{
		op->current_cipher = EVP_aes_128_cfb();
	}
	else
	{
		printf("openssl_set_cipher(): \"%s\" is not a recognized cipher!\n", cipher);
		return false;
	}

	if (!openssl_random(param, KEY_SIZE, op->key))
	{
		printf("openssl_set_cipher(): openssl_random() failed to generate the key!\n");
		return false;
	}

	if (!openssl_random(param, IV_SIZE, op->iv))
	{
		printf("openssl_set_cipher(): openssl_random() failed to generate the IV!\n");
		return false;
	}

	if (!EVP_CipherInit(op->ctx_encrypt, op->current_cipher, op->key, op->iv, 1))
	{
		printf("openssl_set_cipher(): [encrypt] EVP_CipherInit() failed with error: %s\n", openssl_error());
		return false;
	}

	EVP_CIPHER_CTX *ctx = op->ctx_encrypt;

	if (op->current_cipher == EVP_aes_128_ecb())
	{
		if (!EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, op->key, NULL))
		{
			printf("openssl_set_cipher(): EVP_EncryptInit_ex() failed with error: %s\n", openssl_error());
			return 0;
		}
	}
	else if (op->current_cipher == EVP_aes_128_ctr())
	{
		if (!EVP_EncryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, op->key, op->iv))
		{
			printf("openssl_set_cipher(): EVP_EncryptInit_ex() failed with error: %s\n", openssl_error());
			return 0;
		}
	}
	else if (op->current_cipher == EVP_aes_128_cbc())
	{
		if (!EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, op->key, op->iv))
		{
			printf("openssl_set_cipher(): EVP_EncryptInit_ex() failed with error: %s\n", openssl_error());
			return 0;
		}
	}
	else if (op->current_cipher == EVP_aes_128_cfb())
	{
		if (!EVP_EncryptInit_ex(ctx, EVP_aes_128_cfb(), NULL, op->key, op->iv))
		{
			printf("openssl_set_cipher(): EVP_EncryptInit_ex() failed with error: %s\n", openssl_error());
			return 0;
		}
	}
	return true;
}

/**
 * Encrypt data.
 * @param param A pointer to the cryptographic context.
 * @param size The size of the source data.
 * @param dst A pointer to the destination buffer for the encrypted data.
 * @param src A pointer to the source data to be encrypted.
 * @return The size of the encrypted data or an error code.
 */
size_t openssl_encrypt(void *param, const size_t size, void *dst, const void *src)
{
	if (!param || !dst || !src)
	{
		return 0;
	}

	OpenSSLParam *op = param;
	EVP_CIPHER_CTX *ctx = op->ctx_encrypt;

	if (ctx == NULL)
	{
		printf("openssl_encrypt(): EVP context is NULL\n");
		return 0;
	}

	if (!EVP_EncryptInit_ex(ctx, op->current_cipher, NULL, op->key, op->iv))
	{
		printf("openssl_encrypt(): EVP_EncryptInit_ex() failed with error: %s\n", openssl_error());
		return 0;
	}

	int out;

	if (!EVP_CipherUpdate(ctx, dst, &out, src, MESSAGE_SIZE))
	{
		printf("openssl_encrypt(): EVP_CipherUpdate() failed with error: %s\n", openssl_error());
		return 0;
	}

	int out_2 = 0;

	if (op->current_cipher == EVP_aes_128_ctr() ||
		op->current_cipher == EVP_aes_128_cfb())
	{
		if (!EVP_CipherFinal(ctx, dst + out, &out_2))
		{
			printf("openssl_encrypt(): EVP_CipherFinal() failed with error: %s\n", openssl_error());
		}
	}

	return out + out_2;
}

/**
 * Prepare OpenSSL to be called by main by defining pointers to functions containing
 * the implementation.
 *
 * @return a pointer to a struct containing function pointers that implement OpenSSL.
 */
const Crypto *get_lib()
{
	static const Crypto crypto = {
		openssl_name,
		openssl_ciphers,
		openssl_init,
		openssl_free,
		openssl_random,
		openssl_set_cipher,
		openssl_encrypt,
	};

	return &crypto;
}