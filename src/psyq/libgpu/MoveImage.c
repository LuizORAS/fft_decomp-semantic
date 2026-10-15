/* LIBGPU 800249c4-80024a88. */
#include "psx/libgpu.h"

/* Copy the VRAM rectangle `rect` to (`x`, `y`) with a GPU copy command, through the queue; returns -1 for an
 * empty rectangle. */
s32 MoveImage(RECT* rect, s32 x, s32 y) {
    checkRECT(g_psyq_gpu_move_image_name, rect);
    if (rect->w == 0 || rect->h == 0)
        return -1;
    g_psyq_gpu_move_packet.source = ((u32*)rect)[0];
    g_psyq_gpu_move_packet.destination = (y << 16) | (x & 0xffff);
    g_psyq_gpu_move_packet.dimensions = ((u32*)rect)[1];
    return g_psyq_gpu_dispatch->enqueue_four(g_psyq_gpu_dispatch->ordering_table, &g_psyq_gpu_move_packet, 20, 0);
}
