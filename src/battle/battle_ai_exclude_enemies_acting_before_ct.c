#include "fft/battle.h"
#include "psx/types.h"

/* For an enemy-targeting ability, drop from the target list each enemy that
 * will act before the ability lands. Returns 1 while a target is left. */
s32 battle_ai_exclude_enemies_acting_before_ct(void) {
    s32 result;
    s32 i;
    battle_ai_data_t* ai;
    battle_ai_data_t* walk;

    result = 0;
    ai = &g_battle_ai_data_base;
    i = 0;
    /* The target walks a pointer that starts at the block base and steps by
     * sizeof(battle_ai_unit_decision_t), reaching unit_decisions[i].enemy_flag as a
     * 0x1834 displacement; a pointer started at unit_decisions adds an addiu. */
    walk = ai;
    do {
        if (ai->targetability.live.unit_targetable[i] != 0) {
            if ((ai->considered_ability.ai_flags.word & BATTLE_AI_ABILITY_FLAG_1_TARGET_ENEMIES)
                && (walk->unit_decisions[0].enemy_flag != 0) && (battle_ai_check_ability_use_based_on_ct(i) != 0)) {
                ai->targetability.live.unit_targetable[i] = 0;
            } else {
                result = 1;
            }
        }
        i += 1;
        walk = (battle_ai_data_t*)((u8*)walk + sizeof(battle_ai_unit_decision_t));
    } while (i < BATTLE_UNIT_SLOT_COUNT);
    return result;
}
