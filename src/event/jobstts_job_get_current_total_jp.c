#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Menu row callback of JOBSTTS's ability list (jobstts_update_ability_list_menu):
 * the total JP that jobstts_job_calculate_current_total_jp cached
 * for the job chosen in the job list. The row is not read. */
s32 jobstts_job_get_current_total_jp(s32 unused_row) {
    return g_jobstts_job_current_total_jp;
}
