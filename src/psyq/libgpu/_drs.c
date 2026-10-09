#include "psx/libgpu.h"

/* LIBGPU, retail 0x800260b0-0x8002634c. Halfword-pixel transfer with CPU remainder and 16-word DMA blocks. */
int _drs(RECT* rect, u32* data) {
    extern volatile psyq_gpu_environment_t g_psyq_gpu_environment; /* The driver reads the environment as volatile. */
    int size;
    int blocks;
    int rest;
    set_alarm();
    limitRange(rect->w, 0, g_psyq_gpu_environment.vram_width);
    limitRange(rect->h, 0, g_psyq_gpu_environment.vram_height);
    size = (rect->w * rect->h + 1) / 2;
    rest = size >> 4;
    if (size <= 0)
        return -1;
    rest = size - ((size >> 4) << 4);
    blocks = size >> 4;
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
    while (rest--) {
        *data++ = *g_psyq_gpu_gp0_port;
    }
    if (blocks) {
        *g_psyq_gpu_gp1_port = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_DMA_DIRECTION) | PSYQ_GPU_DMA_REQUEST_READ;
        *g_psyq_gpu_dma_madr = (u32)data;
        *g_psyq_gpu_dma_bcr = (blocks << 16) | PSYQ_GPU_DMA_IMAGE_BLOCK_WORDS;
        *g_psyq_gpu_dma_chcr = PSYQ_GPU_DMA_BUSY | PSYQ_GPU_DMA_MODE_REQUEST;
    }
    return 0;
}
