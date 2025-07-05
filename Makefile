.DELETE_ON_ERROR:

CC ?= clang
CFLAGS ?= -Wall -Wextra -O2

SRC_DIR := src
INC_DIR := include
ASM_DIR := asm
LIB_DIR := lib
OUT_DIR := outputs

LIB_TARGET := $(LIB_DIR)/libsynth.a
SRC_FILES := $(SRC_DIR)/synth.c
OBJ_FILES := $(SRC_FILES:.c=.o)

.PHONY: lib test run clean

# Ensure required directories exist
$(LIB_DIR):
	@mkdir -p $@

$(OUT_DIR):
	@mkdir -p $@

# Build object files (add -Iinclude for headers)
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c | $(LIB_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

lib: $(LIB_TARGET)

$(LIB_TARGET): $(OBJ_FILES)
	ar rcs $@ $^

test: $(SRC_DIR)/main.o lib | $(OUT_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -o test $< $(LIB_TARGET) -lm

run: test
	./test

clean:
	rm -f $(OBJ_FILES) $(SRC_DIR)/main.o test $(LIB_TARGET) 