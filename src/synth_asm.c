#include "synth.h"

// Forward declaration of assembly routine
void render_asm_constant(float *left, float *right, int nframes);

void render(float *left, float *right, int nframes) {
    render_asm_constant(left, right, nframes);
} 