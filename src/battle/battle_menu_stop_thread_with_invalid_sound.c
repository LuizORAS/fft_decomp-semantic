#include "fft/battle.h"
#include "psx/types.h"

/* Thread entry in g_battle_menu_idle_action_entries: queue the invalid-choice
 * sound and end the thread. */
void battle_menu_stop_thread_with_invalid_sound(void) {
    battle_sound_set_effect_to_invalid();
    battle_thread_exit_current();
}
