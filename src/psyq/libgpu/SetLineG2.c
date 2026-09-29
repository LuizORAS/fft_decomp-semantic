/* LIBGPU 80023df8-80023e0c: initializes only length and command. */
#include "psx/libgpu.h"

void SetLineG2(void* primitive) {
    P_TAG* tag = (P_TAG*)primitive;

    tag->len = PSYQ_GPU_PACKET_WORDS(LINE_G2);
    tag->code = PSYQ_GPU_CODE_LINE_G2;
}
