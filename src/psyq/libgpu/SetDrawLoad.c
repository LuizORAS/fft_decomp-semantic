/* LIBGPU 80023f70-80023fd4. */
#include "psx/libgpu.h"

void SetDrawLoad(DR_LOAD* primitive, RECT* rect) {
    psyq_draw_load_t* packet = (psyq_draw_load_t*)primitive;
    int size = (rect->w * rect->h + 1) / 2;
    int length = size + 4;
    if (size < 1 || size > 11) {
        length = 0;
    }
    packet->cache_flush = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_CLEAR_CACHE);
    ((P_TAG*)packet)->len = length;
    packet->command = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_CPU_TO_VRAM);
    packet->rect.packed.coordinates = ((psyq_gpu_rect_words_t*)rect)->packed.coordinates;
    packet->rect.packed.dimensions = ((psyq_gpu_rect_words_t*)rect)->packed.dimensions;
}
