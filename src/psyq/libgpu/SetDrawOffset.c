/* LIBGPU 80025460-800254a4. */
#include "psx/libgpu.h"

void SetDrawOffset(void* destination, void* position) {
    DR_OFFSET* packet = destination;
    s16* offset = position;
    ((P_TAG*)packet)->len = PSYQ_GPU_PACKET_WORDS(DR_OFFSET);
    packet->code[0] = get_ofs(offset[0], offset[1]);
    packet->code[1] = 0;
}
