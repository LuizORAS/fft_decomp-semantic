/* LIBGPU 80027308-80027428. */
#include "psx/libc.h"
#include "psx/libgpu.h"

int psyq_gpu_decode_tim(u32* tim, TIM_IMAGE* image) {
    int palette_words;
    if (*tim++ != 0x10) {
        return -1;
    }
    image->mode = *tim++;
    if (GetGraphDebug() == 2) {
        printf(g_psyq_gpu_tim_id_format, 0x10);
    }
    if (GetGraphDebug() == 2) {
        printf(g_psyq_gpu_tim_mode_format, image->mode);
    }
    if (GetGraphDebug() == 2) {
        printf(g_psyq_gpu_tim_address_format, tim);
    }
    if (image->mode & 8) {
        palette_words = *tim >> 2;
        image->crect = (RECT*)(tim + 1);
        image->caddr = tim + 3;
        tim += palette_words;
    } else {
        palette_words = 0;
        image->crect = 0;
        image->caddr = 0;
    }
    {
        u32 size = *tim;
        image->prect = (RECT*)(tim + 1);
        image->paddr = tim + 3;
        size >>= 2;
        size += 2;
        return palette_words + size;
    }
}
