/* LIBGPU 80023e6c-80023e8c. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a Gouraud-shaded three-segment polyline (LINE_G4) and the end
 * marker after its last point; the caller fills the other fields. */
void SetLineG4(void* primitive) {
    ((P_TAG*)primitive)->len = PSYQ_GPU_PACKET_WORDS(LINE_G4);
    ((P_TAG*)primitive)->code = PSYQ_GPU_CODE_LINE_G4;
    ((LINE_G4*)primitive)->pad = PSYQ_GPU_POLYLINE_END;
}
