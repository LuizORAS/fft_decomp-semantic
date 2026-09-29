/* LIBGPU 80023d1c-80023d30: initializes only length and command. */
#include "psx/libgpu.h"

void SetPolyFT4(POLY_FT4* primitive) {
    P_TAG* tag = (P_TAG*)primitive;

    tag->len = PSYQ_GPU_PACKET_WORDS(POLY_FT4);
    tag->code = PSYQ_GPU_CODE_POLY_FT4;
}
