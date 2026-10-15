/* LIBGPU 80023ccc-80023ce0: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a textured triangle (POLY_FT3); the caller fills the other
 * fields. */
void SetPolyFT3(void* primitive) {
    P_TAG* tag = primitive;

    tag->len = PSYQ_GPU_PACKET_WORDS(POLY_FT3);
    tag->code = PSYQ_GPU_CODE_POLY_FT3;
}
