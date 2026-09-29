/* LIBGPU 80023e0c-80023e2c. */
#include "psx/libgpu.h"

void SetLineF3(void* primitive) {
    ((P_TAG*)primitive)->len = PSYQ_GPU_PACKET_WORDS(LINE_F3);
    ((P_TAG*)primitive)->code = PSYQ_GPU_CODE_LINE_F3;
    ((LINE_F3*)primitive)->pad = PSYQ_GPU_POLYLINE_END;
}
