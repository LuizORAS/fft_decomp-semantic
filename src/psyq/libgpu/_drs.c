#include "psx/libgpu.h"

/* LIBGPU, retail 0x800260b0-0x8002634c. Halfword-pixel transfer with CPU remainder and 16-word DMA blocks. */
int _drs(void* rectangle, u32 pixels) {
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
    __asm__("" : "=r"(rect), "=r"(data) : "0"(rect), "1"(data)); /* Preserve the two original saved argument copies. */
    set_alarm();
    width = rect->w;
    result = width;
    if (width >= 0) {
        width_limit = &g_psyq_gpu_vram_width;
        __asm__("" : "=r"(width_limit) : "0"(width_limit)); /* Preserve the materialized clamp address. */
        value = *width_limit;
        __asm__("" : "=r"(value) : "0"(value));
        value = (s16)value;
        __asm__("" : "=r"(value) : "0"(value));
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
        __asm__("" : "=r"(height_limit) : "0"(height_limit));
        value = *height_limit;
        __asm__("" : "=r"(value) : "0"(value));
        value = (s16)value;
        __asm__("" : "=r"(value) : "0"(value));
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
    __asm__("" : "=r"(remainder) : "0"(remainder)); /* The retail first quotient and final remainder share s0. */
    block_count = remainder;
    value = block_count << 4;
    __asm__("" : "=r"(value) : "0"(value)); /* The original subtracts its block-size temporary from words. */
    remainder = words - value;
    blocks = block_count;
    while (!(*g_psyq_gpu_gp1_port & PSYQ_GPU_STATUS_READY_COMMAND)) {
        if (get_alarm())
            return -1;
    }
    *g_psyq_gpu_gp1_port = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_DMA_DIRECTION);
    *g_psyq_gpu_gp0_port = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_CLEAR_CACHE);
    *g_psyq_gpu_gp0_port = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_VRAM_TO_CPU);
    *g_psyq_gpu_gp0_port = ((u32*)rect)[0];
    *g_psyq_gpu_gp0_port = ((u32*)rect)[1];
    while (!(*g_psyq_gpu_gp1_port & PSYQ_GPU_STATUS_READ_DATA_READY)) {
        if (get_alarm())
            return -1;
    }
    while (remainder--) {
        *data++ = *g_psyq_gpu_gp0_port;
    }
    if (blocks) {
        *g_psyq_gpu_gp1_port = (PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_DMA_DIRECTION) | PSYQ_GPU_DMA_REQUEST_READ);
        *g_psyq_gpu_dma_madr = (u32)data;
        *g_psyq_gpu_dma_bcr = ((u32)blocks << 16) | PSYQ_GPU_DMA_IMAGE_BLOCK_WORDS;
        *g_psyq_gpu_dma_chcr = (PSYQ_GPU_DMA_BUSY | PSYQ_GPU_DMA_MODE_REQUEST);
    }
    return 0;
}
