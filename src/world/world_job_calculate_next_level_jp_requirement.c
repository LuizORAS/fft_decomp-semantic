#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of bunit_job_calculate_next_level_jp_requirement. */
s32 world_job_calculate_next_level_jp_requirement(s32 index) {
    s32 level;

    world_job_get_generic_index(g_world_menu_learn_unlocked_jobs[index]);
    level = g_world_job_cached_level & 0xf;
    if (level >= 8) {
        /* job level 8 = mastered: no next-level requirement */
        g_world_job_cached_level_jp_requirement |= MENU_NUMBER_VALUE_DASHES;
    } else {
        g_world_job_cached_level_jp_requirement = g_main_job_level_jp_requirements[level];
    }
    return g_world_job_cached_level_jp_requirement;
}
