#include "fft/battle_text.h"
#include "psx/types.h"

/* Stores the rectangle at 0x801660ec, where text variables $7E and $7F would be. */
void battle_text_set_substitution_values_7e_7f(s32 x, s32 y, s32 width, s32 height) {
    g_battle_text_substitution_rect.x = x;
    g_battle_text_substitution_rect.y = y;
    g_battle_text_substitution_rect.w = width;
    g_battle_text_substitution_rect.h = height;
}
