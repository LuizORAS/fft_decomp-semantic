#include "fft/event_jobstts.h"
#include "fft/main.h"
#include "fft/menu.h"
#include "psx/types.h"

/* The unit's total JP in the job list's index-th job, cached in g_jobstts_job_current_total_jp: a
 * condition callback of the job list's render commands, read back by jobstts_job_get_current_total_jp. */
s32 jobstts_job_calculate_current_total_jp(s32 index) {
    s32 generic_job = job_get_generic_index(g_menu_job_ids[index]);

    return g_jobstts_job_current_total_jp = g_jobstts_unit_data[0]->career.job_points.total.values[generic_job];
}
