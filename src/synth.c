#include "synth.h"
#include <math.h>

#define SAMPLE_RATE 44100.0f
#define TWO_PI 6.283185307179586476925286766559f

static float phase = 0.0f;
static const float freq = 440.0f; // A4

void render(float *left, float *right, int nframes) {
    for (int i = 0; i < nframes; ++i) {
        float sample = sinf(phase);
        phase += TWO_PI * freq / SAMPLE_RATE;
        if (phase >= TWO_PI) {
            phase -= TWO_PI;
        }
        left[i] = sample;
        right[i] = sample;
    }
} 