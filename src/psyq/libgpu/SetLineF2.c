/* LIBGPU 80023de4-80023df8: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a flat-shaded line (LINE_F2); the caller fills the other
 * fields. About 13 functions use it. */
void SetLineF2(void* primitive) {
    P_TAG* tag = primitive;

    tag->len = PSYQ_GPU_PACKET_WORDS(LINE_F2);
    tag->code = PSYQ_GPU_CODE_LINE_F2;
}
