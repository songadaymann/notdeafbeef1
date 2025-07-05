#ifndef SYNTH_H
#define SYNTH_H

// Render nframes of audio into left and right buffers (32-bit float, 
// \
// [-1.0, 1.0] range expected).
void render(float *left, float *right, int nframes);

#endif // SYNTH_H 