/* LIBGPU 80023cb8-80023ccc: initializes only length and command. */
#include "psx/libgpu.h"

void SetPolyF3(POLY_F3* primitive) {
    P_TAG* tag = (P_TAG*)primitive;

    tag->len = PSYQ_GPU_PACKET_WORDS(POLY_F3);
    tag->code = PSYQ_GPU_CODE_POLY_F3;
}
