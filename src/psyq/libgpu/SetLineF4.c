/* LIBGPU 80023e4c-80023e6c. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a flat-shaded three-segment polyline (LINE_F4) and the end
 * marker after its last point; the caller fills the other fields. */
void SetLineF4(void* primitive) {
    ((P_TAG*)primitive)->len = PSYQ_GPU_PACKET_WORDS(LINE_F4);
    ((P_TAG*)primitive)->code = PSYQ_GPU_CODE_LINE_F4;
    ((LINE_F4*)primitive)->pad = PSYQ_GPU_POLYLINE_END;
}
