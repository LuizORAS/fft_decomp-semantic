#include "fft/battle.h"

/* Save g_battle_effect_gravity in g_battle_effect_saved_gravity. A secondary-effect handler saves the
 * effect file's gravity, sets its own while it steps its slots, then puts it back
 * (battle_effect_restore_gravity). */
void battle_effect_save_gravity(void) {
    g_battle_effect_saved_gravity = g_battle_effect_gravity;
}
