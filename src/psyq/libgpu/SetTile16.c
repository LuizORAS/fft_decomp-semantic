/* LIBGPU 80023dbc-80023dd0: initializes only length and command. */
#include "psx/libgpu.h"

void SetTile16(void* primitive) {
    P_TAG* tag = (P_TAG*)primitive;

    tag->len = PSYQ_GPU_FIXED_TILE_WORDS;
    tag->code = PSYQ_GPU_CODE_TILE16;
}
