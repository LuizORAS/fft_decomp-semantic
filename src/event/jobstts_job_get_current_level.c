#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Menu row callback of JOBSTTS's ability list (jobstts_update_ability_list_menu):
 * the job level that jobstts_job_calculate_current_level cached
 * for the job chosen in the job list. The row is not read. */
s32 jobstts_job_get_current_level(s32 unused_row) {
    return g_jobstts_job_current_level;
}
