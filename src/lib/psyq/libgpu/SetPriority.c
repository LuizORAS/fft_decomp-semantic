#include "psx/libgpu.h"

/* Build a mask-bit packet: test the mask bit before drawing (`test_mask`) and set it on drawn pixels
 * (`set_mask`). No code refers to it. */
void SetPriority(DR_PRIO* packet, int test_mask, int set_mask) {
    setlen(packet, PSYQ_GPU_PACKET_WORDS(DR_PRIO));
    packet->code[0] = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_MASK) | (test_mask ? PSYQ_GPU_MASK_TEST : 0)
        | (set_mask ? PSYQ_GPU_MASK_SET : 0);
    packet->code[1] = 0;
}
