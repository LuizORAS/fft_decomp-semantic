#include "fft/battle.h"
#include "psx/types.h"

/* Source rectangles in the shared menu glyph image. */
extern battle_menu_status_panel_glyph_t g_battle_menu_stacked_glyph_lower;
extern battle_menu_status_panel_glyph_t g_battle_menu_stacked_glyph_upper;

void battle_menu_draw_stacked_glyph_pair(void* pixels, world_glyph_blit_t* position) {
    position->rect.x -= 2;
    position->rect.y += 2;
    blit_text_glyph(g_battle_menu_glyph_image, pixels, &g_battle_menu_stacked_glyph_upper, position);
    position->rect.y += 4;
    blit_text_glyph(g_battle_menu_glyph_image, pixels, &g_battle_menu_stacked_glyph_lower, position);
}
