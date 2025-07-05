.DELETE_ON_ERROR:

CC ?= clang
CFLAGS ?= -Wall -Wextra -O2

SRC_DIR := src
INC_DIR := include
ASM_DIR := asm

OUT_C_DIR := outputs/c
OUT_ASM_DIR := outputs/asm

REF_WAV := $(OUT_C_DIR)/output.wav
ASM_WAV := $(OUT_ASM_DIR)/output.wav

# Object files for C build
C_OBJS := $(SRC_DIR)/main_c.o $(SRC_DIR)/synth_c.o

# Object files for ASM build
ASM_OBJS := $(SRC_DIR)/main_asm.o $(SRC_DIR)/synth_asm.o $(ASM_DIR)/render_constant.o

.PHONY: run_c run_asm compare clean dirs

# Ensure output directories exist

dirs:
	@mkdir -p $(OUT_C_DIR) $(OUT_ASM_DIR)

########################################
# Pattern rules

$(SRC_DIR)/main_c.o: $(SRC_DIR)/main.c | dirs
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(SRC_DIR)/main_asm.o: $(SRC_DIR)/main.c | dirs
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@ -DASM_VERSION=1

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c | dirs
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(ASM_DIR)/%.o: $(ASM_DIR)/%.s | dirs
	$(CC) $(CFLAGS) -c $< -o $@

########################################
# Targets

run_c: test_c
	./test_c
	rsync -a test_c $(OUT_C_DIR)/ # optional copy binary

test_c: $(C_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

run_asm: test_asm
	./test_asm

test_asm: $(ASM_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

compare: run_c run_asm compare_audio
	./compare_audio $(REF_WAV) $(ASM_WAV)

compare_audio: $(SRC_DIR)/compare_audio.c
	$(CC) $(CFLAGS) -o $@ $< -lm

clean:
	rm -f $(SRC_DIR)/*.o $(ASM_DIR)/*.o test_c test_asm compare_audio
	rm -rf $(OUT_C_DIR) $(OUT_ASM_DIR) 