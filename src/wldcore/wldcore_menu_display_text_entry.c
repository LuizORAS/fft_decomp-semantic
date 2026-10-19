#include "fft/wldcore.h"

/* Draw text `text_id` at `point` into `buffer`, the window image of render record `index`
 * (world_menu_display_text_entry, with the record's image width as the stride).
 *
 * `point` is passed by value: GCC spills the two register values to their home slots before narrowing
 * them into the origin record. */
void wldcore_menu_display_text_entry(s32 index, s32 text_id, open_point32_t point, u32* buffer) {
    world_glyph_blit_t origin;

    origin.rect.x = point.x;
    origin.rect.y = point.y;
    origin.stride = g_wldcore_window_render_records[index].bounds.w;
    world_menu_display_text_entry(text_id, (u8*)buffer, &origin);
}
