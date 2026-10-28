#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_target.h"

/* Grant the source unit the EXP and JP of its action (battle_unit_apply_action_rewards; a refusal
 * zeroes the amounts shown), after syncing riders with their mounts; set up the EXP/JP display and
 * start the reports at phase 0 with the cursor on the unit (battle_action_init_learn_ability_on_hit).
 * RESUME_ATTACK_PHASE calls it once the effect messages are done. */
void battle_action_grant_rewards(void) {
    battle_unit_t* unit;
    battle_stats_t* stats;

    unit = battle_unit_get_source();
    battle_unit_restore_all_facings_after_attack();
    if (unit != 0) {
        stats = unit->stats;
        if (stats != 0 && battle_unit_apply_action_rewards(stats->battle_id, &unit->strike.action_rewards) == -1) {
            unit->strike.action_rewards.job_level_for_display = 0;
            unit->strike.action_rewards.level_for_display = 0;
            unit->strike.action_rewards.earned_jp = 0;
            unit->strike.action_rewards.earned_experience = 0;
        }
    }
    battle_gfx_init_earned_exp_jp_display(unit);
    g_battle_action_post_action_display_phase = BATTLE_POST_ACTION_DISPLAY_LEVEL_UP;
    battle_target_move_cursor_to_unit(unit);
    battle_action_init_learn_ability_on_hit();
    g_battle_action_post_action = 0;
    g_battle_state_animation_continue_check = 0;
}
