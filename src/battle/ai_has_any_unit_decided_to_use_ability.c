#include "fft/battle_ai.h"

/* Keep in the target list only the units the considered ability is useful on.
 * Returns 1 while one is left. */
s32 ai_has_any_unit_decided_to_use_ability(void) {
    ai_data_t* ai;
    s32 result;
    s32 i;

    result = 0;
    ai = &g_ai_data_base;
    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        if (ai->targetability.live.unit_targetable[i] != 0) {
            if (ai_classify_ability_effect(i, 1) == AI_ABILITY_EFFECT_USEFUL) {
                result = 1;
            } else {
                ai->targetability.live.unit_targetable[i] = 0;
            }
        }
    }
    return result;
}
