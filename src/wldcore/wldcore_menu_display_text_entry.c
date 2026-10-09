#include "fft/wldcore.h"

/* `point` is passed by value: GCC spills the two register words to their
 * home slots before narrowing them into the origin record. */
void wldcore_menu_display_text_entry(s32 index, s32 text_id, wldcore_point32_t point, void* buffer) {
    world_glyph_blit_t origin;
    s32 x = point.x;
    s32 y = point.y;

    origin.rect.x = x;
    origin.rect.y = y;
    origin.stride = (s16)g_wldcore_window_render_records[index].bounds.dimensions.x;
    world_menu_display_text_entry(text_id, buffer, &origin);
}
