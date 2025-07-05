.DELETE_ON_ERROR:

CC ?= clang
CFLAGS ?= -Wall -Wextra -O2

SRC_DIR := src
INC_DIR := include
ASM_DIR := asm
LIB_DIR := lib
OUT_DIR := outputs/asm

# Optional: also create C outputs dir for future use
OUTPUTS_C_DIR := outputs/c

LIB_TARGET := $(LIB_DIR)/libsynth.a
SRC_FILES := $(SRC_DIR)/synth.c
ASM_FILES := $(wildcard $(ASM_DIR)/*.s)
OBJ_FILES := $(SRC_FILES:.c=.o) $(ASM_FILES:.s=.o)

.PHONY: lib test run clean

# Ensure required directories exist
$(LIB_DIR):
	@mkdir -p $@

$(OUT_DIR):
	@mkdir -p $@
	@mkdir -p $(OUTPUTS_C_DIR)

# Build object files (add -Iinclude for headers)
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c | $(LIB_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Rule for ASM objects
$(ASM_DIR)/%.o: $(ASM_DIR)/%.s | $(LIB_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

lib: $(LIB_TARGET)

$(LIB_TARGET): $(OBJ_FILES)
	@mkdir -p $(dir $@)
	ar rcs $@ $^

test: $(SRC_DIR)/main.o lib | $(OUT_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -o test $< $(LIB_TARGET) -lm

run: test
	./test

clean:
	rm -f $(OBJ_FILES) $(SRC_DIR)/main.o test $(LIB_TARGET) 