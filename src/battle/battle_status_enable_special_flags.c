#include "fft/battle.h"

/* Passes a status change on to the unit's display flags
 * (battle_status_queue_misc_graphics_flag_change), but only while an action is
 * executing: previews and AI simulations change no display state.
 *
 * status_id is the status handler index (status id + 1). */
void battle_status_enable_special_flags(s32 status_id, s32 enabled, s32 misc_unit_id) {
    if (g_battle_action_state == BATTLE_ACTION_STATE_EXECUTE) {
        battle_status_queue_misc_graphics_flag_change(status_id, enabled, misc_unit_id);
    }
}
