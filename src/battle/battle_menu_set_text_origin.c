#include "fft/battle.h"
#include "psx/types.h"

/* Store the top-left text origin used by the BATTLE menu renderer. */
void battle_menu_set_text_origin(s32 x, s32 y) {
    g_menu_text_state.origin_x = x;
    g_menu_text_state.origin_y = y;
}
