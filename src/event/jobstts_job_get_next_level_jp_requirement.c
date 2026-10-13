#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Menu row callback of JOBSTTS's ability list (jobstts_ability_run_list_menu):
 * the next level's JP that jobstts_job_calculate_next_level_jp_requirement cached
 * for the job chosen in the job list. The row is not read. */
s32 jobstts_job_get_next_level_jp_requirement(s32 unused_row) {
    return g_jobstts_job_next_level_jp_requirement;
}
