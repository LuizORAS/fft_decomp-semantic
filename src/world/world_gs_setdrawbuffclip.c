#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Program the active draw buffer's clip rectangle: the shared clip rectangle
 * shifted by that buffer's offsets. */
void world_gs_setdrawbuffclip(void) {
    s32 x;
    s32 y;

    x = g_world_gs_clip_rect.x + g_world_gs_buffer_x[g_world_gs_active_buffer];
    y = g_world_gs_clip_rect.y + g_world_gs_buffer_y[g_world_gs_active_buffer];
    g_world_gs_drawenv.clip.x = x;
    g_world_gs_drawenv.clip.y = y;
    g_world_gs_drawenv.clip.w = g_world_gs_clip_rect.w;
    g_world_gs_drawenv.clip.h = g_world_gs_clip_rect.h;
    PutDrawEnv(&g_world_gs_drawenv);
}
