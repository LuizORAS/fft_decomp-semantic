/* LIBGPU 80025460-800254a4. */
#include "psx/libgpu.h"

void SetDrawOffset(DR_OFFSET* destination, u16* position) {
    DR_OFFSET* packet = destination;
    ((P_TAG*)packet)->len = PSYQ_GPU_PACKET_WORDS(DR_OFFSET);
    /* GPU offsets interpret the SDK halfwords as signed coordinates. */
    packet->code[0] = get_ofs((s16)position[0], (s16)position[1]);
    packet->code[1] = 0;
}
