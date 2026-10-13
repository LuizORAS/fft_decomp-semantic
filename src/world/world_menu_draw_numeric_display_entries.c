#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_draw_numeric_display_entries. */
void world_menu_draw_numeric_display_entries(
    void* pixels, menu_number_entry_t* entries, world_glyph_blit_t* position, s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        position->rect.x = entries->x;
        position->rect.y = entries->y;
        if (entries->kind == 0) {
            world_text_render_decimal_value_or_dashes(*entries->value, entries->format, pixels, position);
        } else if (entries->kind == 1) {
            world_draw_menu_number_glyphs(*entries->value, entries->format, pixels, position);
        }
        entries++;
    }
}
