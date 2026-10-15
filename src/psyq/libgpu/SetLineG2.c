/* LIBGPU 80023df8-80023e0c: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a Gouraud-shaded line (LINE_G2); the caller fills the other
 * fields. */
void SetLineG2(void* primitive) {
    P_TAG* tag = primitive;

    tag->len = PSYQ_GPU_PACKET_WORDS(LINE_G2);
    tag->code = PSYQ_GPU_CODE_LINE_G2;
}
