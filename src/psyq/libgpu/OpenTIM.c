/* LIBGPU 80026fa4-80026fb4. */
#include "psx/libgpu.h"

/* Start reading TIM images at `tim`. main_gfx_load_efc_fnt reads its font image this way. */
int OpenTIM(void* tim) {
    g_psyq_gpu_tim_cursor = tim;
    return 0;
}
