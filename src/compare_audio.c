#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

static long get_file_size(FILE *f) {
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return size;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s reference.wav test.wav\n", argv[0]);
        return 1;
    }

    const char *fileA = argv[1];
    const char *fileB = argv[2];

    FILE *fa = fopen(fileA, "rb");
    FILE *fb = fopen(fileB, "rb");
    if (!fa || !fb) {
        perror("fopen");
        return 1;
    }

    // Skip wav headers (assumes 44-byte PCM header)
    fseek(fa, 44, SEEK_SET);
    fseek(fb, 44, SEEK_SET);

    long sizeA = get_file_size(fa) - 44;
    long sizeB = get_file_size(fb) - 44;

    if (sizeA != sizeB) {
        fprintf(stderr, "Files size mismatch after header (%ld vs %ld)\n", sizeA, sizeB);
        return 1;
    }

    long samples = sizeA / sizeof(int16_t);
    const float inv32768 = 1.0f / 32768.0f;
    double errSum = 0.0;

    for (long i = 0; i < samples; ++i) {
        int16_t sa, sb;
        fread(&sa, sizeof(int16_t), 1, fa);
        fread(&sb, sizeof(int16_t), 1, fb);
        float fa_f = sa * inv32768;
        float fb_f = sb * inv32768;
        errSum += fabsf(fa_f - fb_f);
    }

    fclose(fa);
    fclose(fb);

    double meanErr = errSum / samples;
    printf("Mean absolute error: %.6f\n", meanErr);

    const double THRESH = 0.001; // ~ -60 dB
    if (meanErr < THRESH) {
        printf("PASS\n");
        return 0;
    } else {
        printf("FAIL\n");
        return 1;
    }
} 