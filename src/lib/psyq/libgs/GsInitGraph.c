#include "psx/libgs.h"
#include "psx/types.h"

/* Graphics bring-up: GPU and environments, geometry engine, screen state and the initial draw-buffer clip
 * and offset. No code refers to it, so psyq_gs_init_gpu and psyq_gs_init_gte, which only it calls, never run;
 * WLDCORE uses GsInitGraph2. */
void GsInitGraph(u16 x, u16 y, u16 intmode, u16 dither, u16 vram) {
    psyq_gs_init_gpu(x, y, intmode, dither, vram);
    psyq_gs_init_gte();
    PSDIDX = 0;
    psyq_gs_reset_screen_state(x, y);
    GsSetDrawBuffClip();
    GsSetDrawBuffOffset();
}
