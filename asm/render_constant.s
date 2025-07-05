.section __TEXT,__text,regular,pure_instructions

.globl _render_asm_constant
.p2align 2
_render_asm_constant: // void render_asm_constant(float *left, float *right, int nframes)
    // x0 = left*, x1 = right*, w2 = nframes (int)
    // Load constant 0.1f into s0 (binary representation 0x3dcccccd)
    movz    w3, #0xcccc           // lower 16 bits
    movk    w3, #0x3dcc, lsl #16  // upper 16 bits (0x3d 0xcc)
    fmov    s0, w3
    // Duplicate constant for right channel if needed

    // Loop counter in w4 (frames remaining)
    mov     w4, w2
1:
    // Store to left
    str     s0, [x0], #4
    // Store to right
    str     s0, [x1], #4
    subs    w4, w4, #1
    b.ne    1b
    ret 