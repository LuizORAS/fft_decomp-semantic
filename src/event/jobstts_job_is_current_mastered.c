#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Menu row callback of JOBSTTS's ability list (jobstts_update_ability_list_menu):
 * the mastered flag that jobstts_job_calculate_current_mastered cached
 * for the job chosen in the job list. The row is not read. */
s32 jobstts_job_is_current_mastered(s32 unused_row) {
    return g_jobstts_job_current_mastered;
}
