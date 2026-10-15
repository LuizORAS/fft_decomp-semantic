#include "fft/battle.h"
#include "psx/types.h"

/* Return the last ability id kept in the reaction snapshot (g_battle_action_saved_ability_id, part
 * of g_battle_action_saved_command). Nothing on the disc calls it. */
s16 battle_action_load_last_used_ability(void) {
    return g_battle_action_saved_ability_id;
}
