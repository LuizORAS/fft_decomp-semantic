/* LIBGPU 80023e2c-80023e4c. */
#include "psx/libgpu.h"

void SetLineG3(void* primitive) {
    ((P_TAG*)primitive)->len = PSYQ_GPU_PACKET_WORDS(LINE_G3);
    ((P_TAG*)primitive)->code = PSYQ_GPU_CODE_LINE_G3;
    ((LINE_G3*)primitive)->pad = PSYQ_GPU_POLYLINE_END;
}
