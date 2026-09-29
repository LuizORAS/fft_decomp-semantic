/* LIBGPU 80023d30-80023d44: initializes only length and command. */
#include "psx/libgpu.h"

void SetPolyG4(void* primitive) {
    P_TAG* tag = (P_TAG*)primitive;

    tag->len = PSYQ_GPU_PACKET_WORDS(POLY_G4);
    tag->code = PSYQ_GPU_CODE_POLY_G4;
}
