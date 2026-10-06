#include "fft/battle.h"
#include "psx/types.h"

/* Add the unit's JP change (action.jp_change) to its current job
 * (battle_unit_store_jp_and_calculate_unlocked_jobs), and set job_level_gained_flag when the job's
 * level changed. */
void battle_action_apply_jp_change(battle_stats_t* unit) {
    s32 jp;
    s32 initial_level;
    u8 job;
    u8 misc_unit_id;

    job = unit->job_id;
    misc_unit_id = unit->misc_unit_id;
    initial_level = battle_unit_load_job_level(unit, job, &jp);
    battle_unit_store_jp_and_calculate_unlocked_jobs(
        misc_unit_id, unit->action.jp_change, unit->initial_team_flags & BATTLE_TEAM_MASK, jp);
    if (battle_unit_load_job_level(unit, job, &jp) != initial_level) {
        g_current_ability.job_level_gained_flag = 1;
    }
}
