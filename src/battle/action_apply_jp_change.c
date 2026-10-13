#include "fft/battle_action.h"
#include "psx/types.h"

/* Adds the pending JP change to the unit's job and, if that changed the job
   level, sets the job-level-gained flag so the caller queues the level-up report. */
void action_apply_jp_change(battle_stats_t* unit) {
    s32 jp;
    s32 initial_level;
    u8 job;
    u8 battle_id;

    job = unit->job_id;
    battle_id = unit->battle_id;
    initial_level = battle_unit_load_job_level(unit, job, &jp);
    battle_unit_store_jp_and_calculate_unlocked_jobs(
        battle_id, unit->action.jp_change, unit->initial_team_flags & BATTLE_TEAM_MASK, jp);
    if (battle_unit_load_job_level(unit, job, &jp) != initial_level) {
        g_action_current_ability.job_level_gained_flag = 1;
    }
}
