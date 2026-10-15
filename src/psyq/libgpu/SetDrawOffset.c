/* LIBGPU 80025460-800254a4. */
#include "psx/libgpu.h"

/* Build a drawing-offset packet for the x and y halfwords at `position`. About 27 functions use it. */
void SetDrawOffset(void* destination, void* position) {
    DR_OFFSET* packet = destination;
    s16* offset = position;
    ((P_TAG*)packet)->len = PSYQ_GPU_PACKET_WORDS(DR_OFFSET);
    packet->code[0] = get_ofs(offset[0], offset[1]);
    packet->code[1] = 0;
}
