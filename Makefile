CC=gcc
CFLAGS=-Wall -std=c99

SRC_DIR=src
BUILD_DIR=build

TARGETS=q1 q2 q3

all: $(TARGETS)

$(TARGETS): %: $(SRC_DIR)/%/client/client.c $(SRC_DIR)/%/server/server.c
	@mkdir -p $(BUILD_DIR)/$*
	$(CC) $(CFLAGS) $(SRC_DIR)/$*/client/client.c -o $(BUILD_DIR)/$*/client-$*
	$(CC) $(CFLAGS) $(SRC_DIR)/$*/server/server.c -o $(BUILD_DIR)/$*/server-$*

clean:
	rm -f $(BUILD_DIR)/q1/* $(BUILD_DIR)/q2/* $(BUILD_DIR)/q3/*

