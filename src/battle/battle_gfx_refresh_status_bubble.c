#include "fft/battle.h"
#include "psx/types.h"

/* Turn the unit's status bubble on, restarting its timer, when the unit has a status that shows one
 * (BATTLE_MISC_STATUS_BUBBLE_MASK) or is the unit of the current turn event
 * (g_battle_casting_misc_id); otherwise turn it off. */
void battle_gfx_refresh_status_bubble(battle_unit_misc_data_t* unit) {
    if ((unit->status_flags_1_4 & BATTLE_MISC_STATUS_BUBBLE_MASK) != 0 || unit->unit_id == g_battle_casting_misc_id) {
        unit->status_bubble_active = 1;
        unit->status_bubble_timer = 0;
    } else {
        unit->status_bubble_active = 0;
    }
}
