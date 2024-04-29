CC = gcc
CFLAGS = -g -Wall -DITERATIONS=1000000 # -DMESSAGE_SIZE=4096
LDFLAGS = -lm

CFLAGS_OPENSSL = $(CFLAGS) -I "Libraries/OpenSSL/openssl/build/include/"
LDFLAGS_OPENSSL = $(LDFLAGS) -L "Libraries/OpenSSL/openssl/" -lssl -lcrypto 

CFLAGS_BOTAN = $(CFLAGS) -I "Libraries/Botan/botan/build/include/"
LDFLAGS_BOTAN = $(LDFLAGS) -L "Libraries/Botan/botan/" -lbotan-3 

SRC_DIR = src
LIB_OPENSSL = Libraries/OpenSSL
LIB_BOTAN = Libraries/Botan
OUT_DIR = out

SOURCES_OPENSSL = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(LIB_OPENSSL)/*.c)
OBJECTS_OPENSSL = $(SOURCES_OPENSSL:.c=.o)
EXEC_OPENSSL = $(OUT_DIR)/openssl_benchmark

SOURCES_BOTAN = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(LIB_BOTAN)/*.c)
OBJECTS_BOTAN = $(SOURCES_BOTAN:.c=.o)
EXEC_BOTAN = $(OUT_DIR)/botan_benchmark

.PHONY: all openssl botan

all: openssl botan

openssl: $(EXEC_OPENSSL)

botan: $(EXEC_BOTAN)

$(EXEC_OPENSSL): $(OBJECTS_OPENSSL)
	@mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS_OPENSSL) -o $@ $^ $(LDFLAGS_OPENSSL) 

$(EXEC_BOTAN): $(OBJECTS_BOTAN)
	@mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS_BOTAN) -o $@ $^ $(LDFLAGS_BOTAN) 

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OUT_DIR)/*.o $(EXEC_OPENSSL) $(EXEC_BOTAN) $(LIB_OPENSSL)/*.o $(LIB_BOTAN)/*.o $(SRC_DIR)/*.o
