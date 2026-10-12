#include "fft/battle_ai.h"

/*
 * Simulate an action and finalize its hit-percentage average when requested.
 *
 * Accounting starts only for the acting unit with an unset base hit rate.
 * The 0xff counter disables accounting; that path returns 1 without
 * requiring a valuable target. Counted entries are target/strike pairs.
 */
s32 battle_ai_call_ability_processing(battle_action_command_t* action) {
    battle_ai_data_t* ai;
    s32 base_hit;
    s32 cnt;

    ai = &g_battle_ai_data_base;
    g_battle_ai_data_base.hit_counter = 0xFF;
    if (action->actor_id == g_battle_ai_data_base.acting_unit_id) {
        base_hit = g_battle_ai_data_base.current_action.rank.base_hit_percent;
        g_battle_ai_data_base.ability_processing_done = 1;
        if (base_hit == 0xFF) {
            g_battle_ai_data_base.hit_counter = 0;
            g_battle_ai_data_base.total_hit_percent = 0;
            g_battle_ai_data_base.valuable_target_hit = 0;
        }
    }
    battle_ai_simulate_strikes(action, 0);
    cnt = ai->hit_counter;
    if (cnt != 0xFF) {
        if (ai->valuable_target_hit == 0 || cnt == 0) {
            return 0;
        }
        ai->current_action.rank.base_hit_percent = ai->total_hit_percent / cnt;
    }
    return 1;
}
