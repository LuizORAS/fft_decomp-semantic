#include "fft/event_bunit.h"
#include "psx/types.h"

/* The selected unit's total JP in the job list's index-th job, cached in
 * g_bunit_job_current_total_jp: a row callback of the job list, read back by
 * bunit_job_get_current_total_jp. */
s32 bunit_job_calculate_current_total_jp(s32 index) {
    s32 generic_job;
    bunit_unit_data_t* unit;

    generic_job = bunit_job_get_generic_index(g_bunit_job_ids[index]);
    unit = g_bunit_unit_data[g_bunit_unit_selected_index];
    return g_bunit_job_current_total_jp = unit->total_job_points[generic_job];
}
