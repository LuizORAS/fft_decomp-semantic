#include "fft/event_jobstts.h"
#include "fft/main.h"
#include "psx/types.h"

/* JP needed for the next level of the job whose level
 * jobstts_job_calculate_current_level last cached: the table entry for levels
 * below 8, otherwise the previous value with the dashes flag set. The
 * `index`th job is looked up but its result is not used. */
s32 jobstts_job_calculate_next_level_jp_requirement(s32 index) {
    s32 level;

    job_get_generic_index(g_jobstts_job_ids[index]);
    level = g_jobstts_job_current_level & 0xf;
    if (level >= UNIT_JOB_LEVEL_MAX) {
        g_jobstts_job_next_level_jp_requirement |= MENU_NUMBER_VALUE_DASHES;
    } else {
        g_jobstts_job_next_level_jp_requirement = g_main_job_level_jp_requirements[level];
    }
    return g_jobstts_job_next_level_jp_requirement;
}
