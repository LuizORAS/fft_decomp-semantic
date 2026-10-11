#include "psx/libgpu.h"

/* LIBGPU, retail 0x80025c44-0x80025e5c. Selects fill or tile clearing and restores drawing state. */
int _clr(RECT* rect, u32 color) {
    extern volatile psyq_gpu_environment_t g_psyq_gpu_environment; /* An unqualified RAM view removes native halfword
                                                                      normalization in the bounds clamps. */
    limitRange(rect->w, 0, g_psyq_gpu_environment.vram_width - 1);
    limitRange(rect->h, 0, g_psyq_gpu_environment.vram_height - 1);
    if ((rect->x & 63) || (rect->w & 63)) {
        /* A tile under a full draw area and zero offset, then a second packet that restores the three settings. */
        g_psyq_gpu_clear_packet[0] = ((u32)g_psyq_gpu_clear_restore & PSYQ_GPU_DMA_ADDRESS_MASK) | 0x07000000;
        g_psyq_gpu_clear_packet[1] = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_AREA_TOP_LEFT);
        g_psyq_gpu_clear_packet[2]
            = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_AREA_BOTTOM_RIGHT) | PSYQ_GPU_PARAMETER_MASK;
        g_psyq_gpu_clear_packet[3] = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_OFFSET);
        g_psyq_gpu_clear_packet[4] = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_MASK);
        g_psyq_gpu_clear_packet[5] = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_TILE) | (color & PSYQ_GPU_RGB_MASK);
        /* Component packing replaces the retail GPU word loads with halfword loads. */
        g_psyq_gpu_clear_packet[6] = *(u32*)&rect->x;
        g_psyq_gpu_clear_packet[7] = *(u32*)&rect->w;
        g_psyq_gpu_clear_restore[0] = 0x03ffffff;
        g_psyq_gpu_clear_restore[1]
            = _param(PSYQ_GPU_INFO_DRAW_AREA_TOP_LEFT) | PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_AREA_TOP_LEFT);
        g_psyq_gpu_clear_restore[2] = _param(PSYQ_GPU_INFO_DRAW_AREA_BOTTOM_RIGHT)
            | PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_AREA_BOTTOM_RIGHT);
        g_psyq_gpu_clear_restore[3]
            = _param(PSYQ_GPU_INFO_DRAW_OFFSET) | PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_OFFSET);
    } else {
        g_psyq_gpu_clear_packet[0] = 0x04ffffff;
        g_psyq_gpu_clear_packet[1] = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_MASK);
        g_psyq_gpu_clear_packet[2] = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_BLOCK_FILL) | (color & PSYQ_GPU_RGB_MASK);
        g_psyq_gpu_clear_packet[3] = *(u32*)&rect->x;
        g_psyq_gpu_clear_packet[4] = *(u32*)&rect->w;
    }
    _cwc(g_psyq_gpu_clear_packet);
    return 0;
}
