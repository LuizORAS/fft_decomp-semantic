/* LIBGPU 80023d44-80023d58: initializes only length and command. */
#include "psx/libgpu.h"

void SetPolyGT4(void* primitive) {
    P_TAG* tag = (P_TAG*)primitive;

    tag->len = PSYQ_GPU_PACKET_WORDS(POLY_GT4);
    tag->code = PSYQ_GPU_CODE_POLY_GT4;
}
