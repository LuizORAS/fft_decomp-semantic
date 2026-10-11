#include "fft/main.h"

/* The unit's 24 learned-ability bits for one row of learned_abilities (three
 * bytes per row, the first byte in bits 16..23). skillset_id is the row index: 0
 * for the base job, otherwise the skillset's offset from SKILLSET_ID_BASIC_SKILL. */
u32 main_ability_get_known(battle_stats_t* unit, u32 skillset_id) {
    return (unit->learned_abilities[skillset_id].bits[0] << 16) + (unit->learned_abilities[skillset_id].bits[1] << 8)
        | unit->learned_abilities[skillset_id].bits[2];
}
