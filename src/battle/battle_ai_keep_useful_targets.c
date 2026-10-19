#include "fft/battle_ai.h"

/* Keep in the targetable list only the units the considered ability is useful on
 * (battle_ai_classify_ability_effect, with the Reflect check) and return 1 when any is left. */
s32 battle_ai_keep_useful_targets(void) {
    battle_ai_data_t* ai;
    s32 result;
    s32 i;

    result = 0;
    ai = &g_battle_ai_data_base;
    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        if (ai->targetability.live.unit_targetable[i] != 0) {
            if (battle_ai_classify_ability_effect(i, 1) == BATTLE_AI_ABILITY_EFFECT_USEFUL) {
                result = 1;
            } else {
                ai->targetability.live.unit_targetable[i] = 0;
            }
        }
    }
    return result;
}
