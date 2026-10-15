/* LIBGPU 80023dd0-80023de4: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a rectangle of any size (TILE); the caller fills the other
 * fields. About 25 functions use it. */
void SetTile(void* primitive) {
    P_TAG* tag = primitive;

    tag->len = PSYQ_GPU_PACKET_WORDS(TILE);
    tag->code = PSYQ_GPU_CODE_TILE;
}
