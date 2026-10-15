/* LIBGPU 80023d94-80023da8: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a one-pixel rectangle (TILE_1); the caller fills the other
 * fields. */
void SetTile1(void* primitive) {
    P_TAG* tag = primitive;

    tag->len = PSYQ_GPU_FIXED_TILE_WORDS;
    tag->code = PSYQ_GPU_CODE_TILE1;
}
