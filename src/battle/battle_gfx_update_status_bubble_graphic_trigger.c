#include "fft/battle.h"
#include "psx/types.h"

/* Enables the status bubble, restarting its timer, for a unit with a status that
 * shows a bubble (BATTLE_MISC_STATUS_BUBBLE_MASK) and for the acting unit;
 * disables it for any other unit. */
void battle_gfx_update_status_bubble_graphic_trigger(battle_unit_misc_data_t* unit) {
    if ((unit->status_flags_1_4 & BATTLE_MISC_STATUS_BUBBLE_MASK) != 0
        || unit->unit_id == g_battle_unit_source_misc_id) {
        unit->status_bubble_active = 1;
        unit->status_bubble_timer = 0;
    } else {
        unit->status_bubble_active = 0;
    }
}
