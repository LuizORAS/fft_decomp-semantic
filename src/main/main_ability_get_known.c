#include "fft/main.h"

/* Return one job row of the unit's learned abilities (learned_abilities, three bytes per job
 * row, row 0 for Squire or a special character's own job) as a 24-bit value with slot 0 in the top
 * bit; battle_menu_get_unit_skillset_ability_data reads it. */
u32 main_ability_get_known(const battle_stats_t* unit, u32 job_row) {
    return (unit->career.learned_abilities[job_row][0] << 16) + (unit->career.learned_abilities[job_row][1] << 8)
        | unit->career.learned_abilities[job_row][2];
}
