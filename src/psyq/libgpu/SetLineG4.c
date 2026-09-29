/* LIBGPU 80023e6c-80023e8c. */
#include "psx/libgpu.h"

void SetLineG4(void* primitive) {
    ((P_TAG*)primitive)->len = PSYQ_GPU_PACKET_WORDS(LINE_G4);
    ((P_TAG*)primitive)->code = PSYQ_GPU_CODE_LINE_G4;
    ((LINE_G4*)primitive)->pad = PSYQ_GPU_POLYLINE_END;
}
