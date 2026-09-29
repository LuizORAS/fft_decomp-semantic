/* LIBGPU 80023d58-80023d6c: initializes only length and command. */
#include "psx/libgpu.h"

void SetSprt8(void* primitive) {
    P_TAG* tag = (P_TAG*)primitive;

    tag->len = PSYQ_GPU_FIXED_SPRITE_WORDS;
    tag->code = PSYQ_GPU_CODE_SPRT8;
}
