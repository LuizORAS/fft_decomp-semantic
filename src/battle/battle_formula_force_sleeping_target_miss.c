#include "fft/battle.h"
#include "psx/types.h"

/* A sleeping target fails the action (battle_formula_force_attack_miss); Dance, Song and Talk Skill
 * use it. */
void battle_formula_force_sleeping_target_miss(void) {
    if ((g_battle_action_target->status_sets.current[4] & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_SLEEP)) != 0) {
        battle_formula_force_attack_miss();
    }
}
