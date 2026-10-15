/* LIBGPU 80023d58-80023d6c: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of an 8x8 sprite (SPRT_8); the caller fills the other fields. */
void SetSprt8(void* primitive) {
    P_TAG* tag = primitive;

    tag->len = PSYQ_GPU_FIXED_SPRITE_WORDS;
    tag->code = PSYQ_GPU_CODE_SPRT8;
}
