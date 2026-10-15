#include "fft/wldcore.h"

/* Prepare a render record for an image that is already in VRAM.
 *
 * Caller flags replace bits 0x18; texture coordinates use doubled 4bpp units.
 */
void wldcore_window_init_vram_render_record(
    s32 index, wldcore_window_render_bounds16_t bounds, wldcore_point32_t clut, s32 flags) {
    wldcore_window_render_bounds16_t* record_bounds;
    CVECTOR* rgb;

    g_wldcore_window_render_records[index].flags
        = (g_wldcore_window_render_records[index].flags
              & ~(WLDCORE_DISPLAY_OBJECT_BLANK | WLDCORE_DISPLAY_OBJECT_HIDDEN))
        | flags;
    g_wldcore_window_render_records[index].clut = clut;
    g_wldcore_window_render_records[index].tpage = GetTPage(0, 0, bounds.position.x, bounds.position.y) & 0xFFFF;
    record_bounds = &g_wldcore_window_render_records[index].bounds;
    record_bounds->position.x = (bounds.position.x & 0x3F) * 2;
    record_bounds->position.y = bounds.position.y & 0xFF;
    record_bounds->dimensions.x = bounds.dimensions.x * 2;
    record_bounds->dimensions.y = bounds.dimensions.y;
    rgb = &g_wldcore_window_render_records[index].color;
    rgb->r = 0x80;
    rgb->g = 0x80;
    rgb->b = 0x80;
    g_wldcore_window_render_records[index].anim_counter = 0;
}
