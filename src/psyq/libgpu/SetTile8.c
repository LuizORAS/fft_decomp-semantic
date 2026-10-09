/* LIBGPU 80023da8-80023dbc: initializes only length and command. */
#include "psx/libgpu.h"

void SetTile8(void* primitive) {
    P_TAG* tag = primitive;

    tag->len = PSYQ_GPU_FIXED_TILE_WORDS;
    tag->code = PSYQ_GPU_CODE_TILE8;
}
