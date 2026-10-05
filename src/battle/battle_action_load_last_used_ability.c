#include "fft/battle.h"
#include "psx/types.h"

/* Return the last ability id kept in the reaction snapshot (g_reaction_unit_last_ability_id, part
 * of g_reaction_unit_action_data_16e). Nothing on the disc calls it. */
s16 battle_action_load_last_used_ability(void) {
    return g_reaction_unit_last_ability_id;
}
