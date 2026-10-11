#include "fft/open.h"
#include "psx/types.h"

/* Draw one birthday menu entry into the window's scratch image. */
void open_birthday_draw_menu_text_entry(s32 record_index, s32 combined_text_index, point32_t position, void* image) {
    glyph_blit_t origin;
    s32 x = position.x;
    s32 y = position.y;

    origin.rect.x = x;
    origin.rect.y = y;
    origin.stride = g_open_gfx_render_records_56[record_index].tail.quad.w;
    world_menu_display_text_entry(combined_text_index, image, &origin);
}
