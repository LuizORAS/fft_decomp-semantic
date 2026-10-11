/* LIBGPU 80023e8c-80023ea0: initializes only length and command. */
#include "psx/libgpu.h"

void SetBlockFill(TILE* primitive) {
    setlen(primitive, PSYQ_GPU_PACKET_WORDS(TILE));
    setcode(primitive, PSYQ_GPU_CODE_BLOCK_FILL);
}
