/* LIBGPU 80023ccc-80023ce0: initializes only length and command. */
#include "psx/libgpu.h"

void SetPolyFT3(void* primitive) {
    P_TAG* tag = (P_TAG*)primitive;

    tag->len = PSYQ_GPU_PACKET_WORDS(POLY_FT3);
    tag->code = PSYQ_GPU_CODE_POLY_FT3;
}
