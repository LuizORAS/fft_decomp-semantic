#include "fft/battle.h"

/* Queue the confirm sound unless a menu restore is pending
 * (g_battle_menu_restore_pending). */
void battle_sound_set_effect_to_confirm_checked(void) {
    if (g_battle_menu_restore_pending == 0) {
        g_battle_sound_pending_sfx_id = MAIN_SFX_CONFIRM;
    }
}
