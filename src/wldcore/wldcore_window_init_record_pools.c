#include "fft/wldcore.h"
#include "psx/types.h"

/* Reset both world-map window pools and their allocation counters. */
void wldcore_window_init_record_pools(void) {
    CVECTOR* window_color;
    point32_t* render_geometry;
    CVECTOR* render_color;
    CVECTOR* current_window_color;
    CVECTOR* current_render_color;
    point32_t* current_geometry;
    s32 i;

    /* Direct record-field access changes the column bases and instruction scheduling. */
    window_color = &g_wldcore_window_records->color;
    g_wldcore_window_render_record_count = 0;
    g_wldcore_window_record_count = 0;
    g_wldcore_window_render_object_count = 0;
    g_wldcore_window_aux_render_object_count = 0;
    for (i = 0; i < WLDCORE_WINDOW_RECORD_CAPACITY; i++) {
        g_wldcore_window_records[i].flags = WORLD_RENDER_RECORD_ANIMATED;
        g_wldcore_window_records[i].priority = 1;
        current_window_color = (CVECTOR*)((u8*)window_color + i * sizeof(g_wldcore_window_records[0]));
        current_window_color->r = 0x80;
        current_window_color->g = 0x80;
        current_window_color->b = 0x80;
    }

    render_geometry = &g_wldcore_window_render_records->clut;
    render_color = &g_wldcore_window_render_records->color;
    for (i = 0; i < WLDCORE_WINDOW_RECORD_CAPACITY; i++) {
        g_wldcore_window_render_records[i].flags = WORLD_RENDER_RECORD_IMAGE;
        g_wldcore_window_render_records[i].priority = 1;
        current_geometry = (point32_t*)((u8*)render_geometry + i * sizeof(g_wldcore_window_render_records[0]));
        current_geometry->x = 0x3c0;
        current_geometry->y = 0x1f0;
        current_render_color = (CVECTOR*)((u8*)render_color + i * sizeof(g_wldcore_window_render_records[0]));
        current_render_color->r = 0x80;
        current_render_color->g = 0x80;
        current_render_color->b = 0x80;
    }
}
