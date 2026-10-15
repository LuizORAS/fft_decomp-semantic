#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x59, Level Blast: the magical evade check and the hit chance MA + X without Faith, then
 * the target loses a level (LEVEL_DOWN); a target below level 2 fails, though the flag is still
 * written. */
void battle_formula_lower_level_by_one(void) {
    battle_action_data_t* action;

    if (battle_formula_calculate_magical_evade() == 0) {
        if (battle_formula_calculate_magic_accuracy_without_faith() == 0) {
            if (g_battle_action_target->level < 2) {
                battle_formula_force_attack_miss();
            }
            action = g_battle_action_target_data;
            /* The target writes the combined special-effect field as one halfword. */
            action->special_effect = BATTLE_ACTION_SPECIAL_EFFECT_LEVEL_DOWN;
            action->attack_type = BATTLE_ACTION_TYPE_PSEUDO_STATUS;
        }
    }
}
