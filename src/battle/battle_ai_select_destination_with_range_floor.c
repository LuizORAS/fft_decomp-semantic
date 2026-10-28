#include "fft/battle_ai.h"

/* Choose the destination within distance_limit of the decision target, raised to 3 for a ranged unit
 * (BATTLE_AI_TARGETING_FLAG_1_HAS_RANGED_ABILITY) against an enemy target so it may stop 3 tiles away,
 * else the nearest tile (battle_ai_select_destination_with_nearest_fallback). */
void battle_ai_select_destination_with_range_floor(s32 distance_limit) {
    battle_ai_unit_decision_t* decision = g_battle_ai_data_base.acting_unit_decision;
    if (g_battle_ai_data_base.unit_decisions[decision->main_target_id].enemy_flag != 0
        && (decision->targeting_flags_1 & BATTLE_AI_TARGETING_FLAG_1_HAS_RANGED_ABILITY) && distance_limit < 3) {
        distance_limit = 3;
    }
    battle_ai_select_destination_with_nearest_fallback(distance_limit);
}
