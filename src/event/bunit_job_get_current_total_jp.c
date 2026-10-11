#include "fft/event_bunit.h"
#include "psx/types.h"

/* Menu row callback of BUNIT's ability list (bunit_run_ability_list_menu):
 * the total JP that bunit_job_calculate_current_total_jp cached
 * for the job chosen in the job list. The row is not read. */
s32 bunit_job_get_current_total_jp(s32 unused_row) {
    return g_bunit_job_current_total_jp;
}
