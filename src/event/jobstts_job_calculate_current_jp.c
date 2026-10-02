#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Also returns the value: callers read it through g_jobstts_cmd_conditions. */
u32 jobstts_job_calculate_current_jp(s32 index) {
    s32 generic_job = jobstts_job_get_generic_index(g_jobstts_job_ids[index]);

    return g_jobstts_job_current_jp = g_jobstts_unit_data[0]->job_points[generic_job];
}
