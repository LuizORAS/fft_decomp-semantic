#include "fft/battle.h"
#include "psx/types.h"

/* Store a rectangle as four halfwords in g_battle_text_substitution_rect, the two words where text
 * control 0xE6 (TEXT_PRINT_INDEXED_VALUE) would find substitution values $7E and $7F, past the 32 of
 * g_battle_text_substitution_values. Nothing calls it. */
void battle_text_set_substitution_values_7e_7f(s32 x, s32 y, s32 width, s32 height) {
    g_battle_text_substitution_rect.x = x;
    g_battle_text_substitution_rect.y = y;
    g_battle_text_substitution_rect.w = width;
    g_battle_text_substitution_rect.h = height;
}
