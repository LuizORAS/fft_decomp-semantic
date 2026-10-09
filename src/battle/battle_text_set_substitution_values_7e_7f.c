#include "fft/battle.h"
#include "psx/types.h"

/* Stores the rectangle at 0x801660ec, where text variables $7E and $7F would be. */
void battle_text_set_substitution_values_7e_7f(s32 x, s32 y, s32 width, s32 height) {
    D_801660EC.x = x;
    D_801660EC.y = y;
    D_801660EC.w = width;
    D_801660EC.h = height;
}
