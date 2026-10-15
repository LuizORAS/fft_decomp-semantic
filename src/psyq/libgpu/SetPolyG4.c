/* LIBGPU 80023d30-80023d44: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a Gouraud-shaded quadrilateral (POLY_G4); the caller fills the
 * other fields. About 13 functions use it. */
void SetPolyG4(void* primitive) {
    P_TAG* tag = primitive;

    tag->len = PSYQ_GPU_PACKET_WORDS(POLY_G4);
    tag->code = PSYQ_GPU_CODE_POLY_G4;
}
