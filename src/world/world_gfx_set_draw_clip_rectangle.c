#include "fft/world.h"
#include "psx/libgpu.h"

/* Copy the clip rectangle that DRAWENV setup reads (libgs GsSetClip). */
void world_gfx_set_draw_clip_rectangle(RECT* source) {
    g_world_gs_clip_rect.x = source->x;
    g_world_gs_clip_rect.y = source->y;
    g_world_gs_clip_rect.w = source->w;
    g_world_gs_clip_rect.h = source->h;
}
