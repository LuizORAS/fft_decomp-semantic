/* LIBGPU 80023de4-80023df8: initializes only length and command. */
#include "psx/libgpu.h"

void SetLineF2(void* primitive) {
    P_TAG* tag = (P_TAG*)primitive;

    tag->len = PSYQ_GPU_PACKET_WORDS(LINE_F2);
    tag->code = PSYQ_GPU_CODE_LINE_F2;
}
