# CBOS Template

This is a template for benchmarking various cryptographic ciphers in C. This template provides a basic structure for creating and benchmarking your own cryptographic library. It includes placeholders for defining and implementing ciphers, initialization functions, encryption, and more.

## Benchmarking your own code.c

To benchmark a cryptographic library, it's necessary to implement the functions specified in the Crypto struct found in [cbos.h](../src/cbos.h) within a separate file.

To begin, define the following macros: 

+ `KEY_SIZE`

+ `IV_SIZE`,

+ `MESSAGE_SIZE`,

+ `ITERATION`.

Include the `cbos.` interface header file and any other necessary header files that you might need. 

You can refer to the example template provided in [template.h](template.h) and [template.c](template.c) for guidance.

### Important Note:

When including headers in a different file, be aware that the following headers are already included in `cbos.h`. Including them in a different file might produce an error due to the use of `#pragma once` in the original headers.

```c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
```
