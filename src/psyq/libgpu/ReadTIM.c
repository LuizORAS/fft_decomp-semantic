/* LIBGPU 80026fb4-8002701c. */
#include "psx/libgpu.h"

int ReadTIM(TIM_IMAGE* image) {
    int size = psyq_gpu_decode_tim(g_psyq_gpu_tim_cursor, image);
    register int result __asm__("$2"); /* A separate return temporary changes v0 allocation. */
    if (size != -1) {
        g_psyq_gpu_tim_cursor += size;
        result = (int)image;
    } else {
        result = 0;
    }
    return result;
}
