/* LIBGPU 80023f70-80023fd4. */
#include "psx/libgpu.h"

/* Prepare a packet that loads a small image into the VRAM rectangle `rect`: a cache clear, the CPU-to-VRAM
 * command and the rectangle, with a length counting the 32-bit pixel values the caller adds (QUIRKS.md); length 0 for
 * an image of more than 11 32-bit values. No code refers to it. */
void SetDrawLoad(DR_LOAD* packet, RECT* rect) {
    int size = (rect->w * rect->h + 1) / 2;
    int length = size + 4;
    if (size < 1 || size > 11) {
        length = 0;
    }
    packet->code[0] = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_CLEAR_CACHE);
    setlen(packet, length);
    packet->code[1] = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_CPU_TO_VRAM);
    /* Component packing replaces the retail 32-bit GPU loads with 16-bit loads. */
    packet->code[2] = *(u32*)&rect->x;
    packet->code[3] = *(u32*)&rect->w;
}
