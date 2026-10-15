/* LIBGPU 80023dbc-80023dd0: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a 16x16 rectangle (TILE_16); the caller fills the other fields. */
void SetTile16(void* primitive) {
    P_TAG* tag = primitive;

    tag->len = PSYQ_GPU_FIXED_TILE_WORDS;
    tag->code = PSYQ_GPU_CODE_TILE16;
}
