#pragma once

// Define the name of the ciphers you want to benchmark.
#define SOME_CIPHER "CIPHER-key-MODE-Example"

// The name of the following macros must not be changed.
#define KEY_SIZE 32
#define IV_SIZE 32

#define MESSAGE_SIZE 4096
#define ITERATIONS 1000000

#include "../src/cbos.h" 

// Include some other headers that you need here

typedef struct CipherParam
{
	unsigned char key[KEY_SIZE];
	unsigned char iv[IV_SIZE];
	//some other parameters come here
} CipherParam;

/**
 * The function returns a structure with pointers 
 * to functions that implement the features of the library.
 */ 
const Crypto *mylib_get();

//Append function implementations in the following