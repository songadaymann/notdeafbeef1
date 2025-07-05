#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "synth.h"

#define SAMPLE_RATE 44100
#define DURATION_SECONDS 2
#define OUTPUT_FILE "outputs/asm/output.wav"

static void write_wav(const char *filename, const float *left, const float *right, int nframes);

int main(void) {
    int frames = SAMPLE_RATE * DURATION_SECONDS;
    float *left = (float *)malloc(sizeof(float) * frames);
    float *right = (float *)malloc(sizeof(float) * frames);
    if (!left || !right) {
        fprintf(stderr, "Failed to allocate buffers\n");
        return 1;
    }

    render(left, right, frames);

    // Ensure output directory exists
    system("mkdir -p outputs/asm");
    write_wav(OUTPUT_FILE, left, right, frames);

    free(left);
    free(right);

    printf("Wrote %s (%d frames)\n", OUTPUT_FILE, frames);
    return 0;
}

static void write_wav(const char *filename, const float *left, const float *right, int nframes) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("fopen");
        return;
    }

    int channels = 2;
    int bitsPerSample = 16;
    int byteRate = SAMPLE_RATE * channels * bitsPerSample / 8;
    int blockAlign = channels * bitsPerSample / 8;
    int dataSize = nframes * blockAlign;
    int chunkSize = 36 + dataSize;

    // RIFF header
    fwrite("RIFF", 1, 4, f);
    uint32_t tmp32 = chunkSize; fwrite(&tmp32, 4, 1, f);
    fwrite("WAVE", 1, 4, f);

    // fmt  subchunk
    fwrite("fmt ", 1, 4, f);
    tmp32 = 16; fwrite(&tmp32, 4, 1, f); // PCM
    uint16_t tmp16 = 1; fwrite(&tmp16, 2, 1, f); // Audio format
    tmp16 = channels; fwrite(&tmp16, 2, 1, f);
    tmp32 = SAMPLE_RATE; fwrite(&tmp32, 4, 1, f);
    tmp32 = byteRate; fwrite(&tmp32, 4, 1, f);
    tmp16 = blockAlign; fwrite(&tmp16, 2, 1, f);
    tmp16 = bitsPerSample; fwrite(&tmp16, 2, 1, f);

    // data subchunk
    fwrite("data", 1, 4, f);
    tmp32 = dataSize; fwrite(&tmp32, 4, 1, f);

    // Write samples interleaved L R as int16
    for (int i = 0; i < nframes; ++i) {
        int16_t l = (int16_t)(left[i] * 32767.0f);
        int16_t r = (int16_t)(right[i] * 32767.0f);
        fwrite(&l, sizeof(int16_t), 1, f);
        fwrite(&r, sizeof(int16_t), 1, f);
    }

    fclose(f);
} 