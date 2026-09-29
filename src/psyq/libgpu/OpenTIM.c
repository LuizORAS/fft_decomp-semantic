/* LIBGPU 80026fa4-80026fb4. */
#include "psx/libgpu.h"

int OpenTIM(void* tim) {
    g_psyq_gpu_tim_cursor = tim;
    return 0;
}
