#include "fft/event_bunit.h"
#include "fft/main.h"
#include "fft/menu.h"
#include "psx/types.h"

/* The selected unit's total JP in the job list's index-th job, cached in
 * g_bunit_job_current_total_jp: a row callback of the job list, read back by
 * bunit_job_get_current_total_jp. */
s32 bunit_job_calculate_current_total_jp(s32 index) {
    s32 generic_job;

    generic_job = job_get_generic_index(g_menu_job_ids[index]);
    return g_bunit_job_current_total_jp
        = g_bunit_unit_pointers[g_bunit_unit_selected_index]->job_points.total.values[generic_job];
}
