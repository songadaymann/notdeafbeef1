#include "synth.h"

void render(float *left, float *right, int nframes) {
    const float value = 0.1f;
    for (int i = 0; i < nframes; ++i) {
        left[i] = value;
        right[i] = value;
    }
} 