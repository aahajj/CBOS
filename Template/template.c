#include "template.h"

int get_message_size()
{
	return MESSAGE_SIZE;
}

int get_iterations()
{
	return ITERATIONS;
}

const char *mylib_get_name()
{
	return "MY Crypto Library";
}

const char **mylib_ciphers()
{
	static const char *names[] = {
		SOME_CIPHER,
		NULL
        };

	return names;
}

bool mylib_init(void **param)
{
	//initlize your parameters such as CTX or keys 
	return true;
}

bool mylib_free(void *param)
{
	return true;
}

bool mylib_random(void *param, const size_t size, void *data)
{
	// here you can use the random_bytes(data, size) function from cbos.h
	return true;
}

bool mylib_set_cipher(void *param, const char *cipher)
{
	//prepare the cipher that you want to benchmark
	return true;
}

size_t mylib_encrypt(void *param, const size_t size, void *dst, const void *src)
{
	// call the encryption method in your library
	return 0;
}

const Crypto *get_lib()
{
	static const Crypto crypto = {
		mylib_get_name,
		mylib_ciphers,
		mylib_init,
		mylib_free,
		mylib_random,
		mylib_set_cipher,
		mylib_encrypt,
	};

	return &crypto;
}
