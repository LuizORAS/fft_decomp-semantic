/* LIBGPU 80023ce0-80023cf4: initializes only length and command. */
#include "psx/libgpu.h"

void SetPolyG3(void* primitive) {
    P_TAG* tag = (P_TAG*)primitive;

    tag->len = PSYQ_GPU_PACKET_WORDS(POLY_G3);
    tag->code = PSYQ_GPU_CODE_POLY_G3;
}
