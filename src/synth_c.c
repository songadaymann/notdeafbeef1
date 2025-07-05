#include "synth.h"
#include <math.h>

#define SAMPLE_RATE 44100.0f

// Simple kick drum: exponential frequency sweep and amplitude decay

void render(float *left, float *right, int nframes) {
    const float f_start = 120.0f;   // initial frequency (Hz)
    const float f_end   = 40.0f;    // final frequency (Hz)
    const float sweep_dur = 0.5f;   // seconds over which sweep occurs
    const float amp_tau  = 0.4f;    // amplitude decay time constant (s)

    float phase = 0.0f;
    const float sweep_coeff = logf(f_end / f_start); // negative value

    for (int i = 0; i < nframes; ++i) {
        float t = (float)i / SAMPLE_RATE;

        // Frequency sweeps exponentially from f_start to f_end over sweep_dur
        float ratio = f_end / f_start;
        float sweep_t = fminf(t / sweep_dur, 1.0f);
        float freq = f_start * powf(ratio, sweep_t);

        // Increment phase
        phase += 2.0f * (float)M_PI * freq / SAMPLE_RATE;
        if (phase > 2.0f * (float)M_PI)
            phase -= 2.0f * (float)M_PI;

        // Amplitude envelope exponential decay
        float amp = expf(-t / amp_tau);

        float sample = amp * sinf(phase);
        left[i]  = sample;
        right[i] = sample;
    }
} 