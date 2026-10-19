#include "fft/main.h"

/* Set the unit's ability_ct to the charge time of its last ability (the low seven bits of the
 * range data's CT); the turn clock, the AT list and the action-state change call it. */
void main_unit_copy_last_ability_ct(battle_stats_t* unit) {
    unit->ability_ct = g_main_ability_range_data[unit->action_command.ability_id].ct & 0x7f;
}
