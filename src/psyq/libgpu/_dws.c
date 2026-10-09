#include "psx/libgpu.h"

/* LIBGPU, retail 0x80025e5c-0x800260b0. Halfword-pixel transfer with CPU remainder and 16-word DMA blocks. */
int _dws(void* rectangle, u32 pixels) {
    RECT* rect = rectangle;
    u32* data = (u32*)pixels;
    int words;
    int blocks;
    int remainder;
    /* Bindings retain the retail clamp and DMA word-count registers. */
    register int block_count __asm__("$3");
    register int width __asm__("$4");
    register int height __asm__("$5");
    register int result __asm__("$3");
    register int value __asm__("$2");
    /* Volatile retains separate signed comparisons and unsigned clamp reloads. */
    volatile u16* width_limit;
    register volatile u16* height_limit __asm__("$6");
    int unused_frame[4]; /* Retail reserves sixteen additional unused frame bytes. */
    register int special __asm__("$21");
    __asm__("" : "=r"(rect), "=r"(data) : "0"(rect), "1"(data)); /* Preserve the two original saved argument copies. */
    set_alarm();
    special = 0;
    width = rect->w;
    result = width;
    if (width >= 0) {
        width_limit = &g_psyq_gpu_vram_width;
        value = *width_limit;
        value = (s16)value;
        if (value < width)
            result = *width_limit;
    } else
        result = 0;
    height = rect->h;
    rect->w = result;
    result = height;
    if (height >= 0) {
        __asm__("" : "=r"(result) : "0"(result)); /* Preserve the retail copy of the input height in v1. */
        height_limit = &g_psyq_gpu_vram_height;
        value = *height_limit;
        value = (s16)value;
        if (value < height)
            width = *height_limit;
        else
            width = result;
    } else
        width = 0;
    rect->h = width;
    words = (rect->w * (s16)width + 1) / 2;
    remainder = words >> 4;
    if (words <= 0)
        return -1;
    block_count = remainder;
    value = block_count << 4;
    remainder = words - value;
    blocks = block_count;
    while (!(*g_psyq_gpu_gp1_port & PSYQ_GPU_STATUS_READY_COMMAND)) {
        if (get_alarm())
            return -1;
    }
    *g_psyq_gpu_gp1_port = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_DMA_DIRECTION);
    *g_psyq_gpu_gp0_port = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_CLEAR_CACHE);
    *g_psyq_gpu_gp0_port = special ? 0xb0000000 : PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_CPU_TO_VRAM);
    *g_psyq_gpu_gp0_port = ((u32*)rect)[0];
    *g_psyq_gpu_gp0_port = ((u32*)rect)[1];
    while (remainder--) {
        *g_psyq_gpu_gp0_port = *data++;
    }
    if (blocks) {
        *g_psyq_gpu_gp1_port = (PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_DMA_DIRECTION) | PSYQ_GPU_DMA_REQUEST_WRITE);
        *g_psyq_gpu_dma_madr = (u32)data;
        *g_psyq_gpu_dma_bcr = ((u32)blocks << 16) | PSYQ_GPU_DMA_IMAGE_BLOCK_WORDS;
        *g_psyq_gpu_dma_chcr = (PSYQ_GPU_DMA_BUSY | PSYQ_GPU_DMA_MODE_REQUEST | PSYQ_GPU_DMA_TO_DEVICE);
    }
    return 0;
}
