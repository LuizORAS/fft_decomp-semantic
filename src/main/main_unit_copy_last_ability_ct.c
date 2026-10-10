#include "fft/main.h"

/* Charge time of the unit's last ability: the low seven bits of the ability
 * record's CT byte. */
void main_unit_copy_last_ability_ct(battle_stats_t* unit) {
    s32 ability_id = unit->last_ability_id;

    unit->ability_ct = g_main_ability_ct_data[ability_id * sizeof(ability_secondary_data_t)] & 0x7f;
}
