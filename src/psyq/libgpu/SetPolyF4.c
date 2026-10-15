/* LIBGPU 80023d08-80023d1c: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a flat-shaded quadrilateral (POLY_F4); the caller fills the
 * other fields. About 15 functions use it. */
void SetPolyF4(POLY_F4* primitive) {
    P_TAG* tag = (P_TAG*)primitive;

    tag->len = PSYQ_GPU_PACKET_WORDS(POLY_F4);
    tag->code = PSYQ_GPU_CODE_POLY_F4;
}
