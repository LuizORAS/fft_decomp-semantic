/* LIBGPU 80023e8c-80023ea0: initializes only length and command. */
#include "psx/libgpu.h"

void SetBlockFill(void* primitive) {
    P_TAG* tag = (P_TAG*)primitive;

    tag->len = PSYQ_GPU_PACKET_WORDS(TILE);
    tag->code = PSYQ_GPU_CODE_BLOCK_FILL;
}
