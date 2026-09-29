/* LIBGPU 80023d6c-80023d80: initializes only length and command. */
#include "psx/libgpu.h"

void SetSprt16(void* primitive) {
    P_TAG* tag = (P_TAG*)primitive;

    tag->len = PSYQ_GPU_FIXED_SPRITE_WORDS;
    tag->code = PSYQ_GPU_CODE_SPRT16;
}
