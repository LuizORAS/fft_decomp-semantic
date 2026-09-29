/* LIBGPU 80023d80-80023d94: initializes only length and command. */
#include "psx/libgpu.h"

void SetSprt(void* primitive) {
    P_TAG* tag = (P_TAG*)primitive;

    tag->len = PSYQ_GPU_PACKET_WORDS(SPRT);
    tag->code = PSYQ_GPU_CODE_SPRT;
}
