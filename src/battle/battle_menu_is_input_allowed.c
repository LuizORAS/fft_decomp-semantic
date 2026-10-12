#include "fft/battle.h"

/* Return 1 when the current menu thread may act on input: its child (the thread in the slot below)
 * is not running, it has not been asked to close (third parameter), menu input is not locked (event
 * instruction LOCK_MENU_INPUT, BUNIT's selection), the help is not open and
 * g_battle_menu_input_block_frames is 0 (only WORLD's copy arms it, for a help box). */
s32 battle_menu_is_input_allowed(void) {
    if (battle_thread_is_previous_running() != 0 || battle_thread_get_current_parameter_3() != 0
        || g_battle_menu_input_lock_mode != 0 || g_battle_menu_help_open != 0
        || g_battle_menu_input_block_frames != 0) {
        return 0;
    }
    return 1;
}
