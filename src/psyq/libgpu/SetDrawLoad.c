/* LIBGPU 80023f70-80023fd4. */
#include "psx/libgpu.h"

void SetDrawLoad(void* primitive, RECT* rect) {
    psyq_draw_load_t* packet = primitive;
    int size = (rect->w * rect->h + 1) / 2;
    int length = size + 4;
    if ((u32)(size - 1) >= 11) {
        length = 0;
    }
    packet->cache_flush = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_CLEAR_CACHE);
    ((P_TAG*)packet)->len = length;
    packet->command = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_CPU_TO_VRAM);
    packet->rect.words[0] = ((psyq_gpu_rect_words_t*)rect)->words[0];
    packet->rect.words[1] = ((psyq_gpu_rect_words_t*)rect)->words[1];
}
