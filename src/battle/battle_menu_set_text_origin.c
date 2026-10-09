#include "fft/battle.h"
#include "psx/types.h"

/* Store the top-left text origin used by the BATTLE menu renderer. */
void battle_menu_set_text_origin(s32 x, s32 y) {
    *(s16*)g_battle_menu_text_origin_x = x;
    *(s16*)g_battle_menu_text_origin_y = y;
}
