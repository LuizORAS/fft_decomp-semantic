#include "fft/main.h"

/* Return one job row of the unit's learned abilities (learned_abilities, three bytes per job
 * row, row 0 for Squire or a special character's own job) as a 24-bit value with slot 0 in the top
 * bit; battle_menu_get_unit_skillset_ability_data reads it. */
u32 main_ability_get_known(battle_stats_t* unit, u32 job_row) {
    u32 learned_ability_offset = job_row * UNIT_LEARNED_ABILITY_BYTES_PER_JOB;

    return (unit->learned_abilities[learned_ability_offset] << 16)
        + (unit->learned_abilities[learned_ability_offset + 1] << 8)
        | unit->learned_abilities[learned_ability_offset + 2];
}
