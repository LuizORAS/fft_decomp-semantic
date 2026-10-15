#include "fft/battle.h"

/* Queue a status's graphics change (battle_status_queue_misc_graphics_flag_change: status is the
 * handler index, enabled 0 removes, 1 adds, 2 re-adds) only while executing, so previews and AI
 * simulations change no graphics. */
void battle_status_queue_graphics_change_if_executing(s32 status_id, s32 enabled, s32 misc_unit_id) {
    if (g_battle_action_state == BATTLE_ACTION_STATE_EXECUTE) {
        battle_status_queue_misc_graphics_flag_change(status_id, enabled, misc_unit_id);
    }
}
