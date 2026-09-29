/* LIBGPU 80023dd0-80023de4: initializes only length and command. */
#include "psx/libgpu.h"

void SetTile(void* primitive) {
    P_TAG* tag = (P_TAG*)primitive;

    tag->len = PSYQ_GPU_PACKET_WORDS(TILE);
    tag->code = PSYQ_GPU_CODE_TILE;
}
