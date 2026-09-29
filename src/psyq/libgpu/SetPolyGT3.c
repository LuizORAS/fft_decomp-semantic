/* LIBGPU 80023cf4-80023d08: initializes only length and command. */
#include "psx/libgpu.h"

void SetPolyGT3(void* primitive) {
    P_TAG* tag = (P_TAG*)primitive;

    tag->len = PSYQ_GPU_PACKET_WORDS(POLY_GT3);
    tag->code = PSYQ_GPU_CODE_POLY_GT3;
}
