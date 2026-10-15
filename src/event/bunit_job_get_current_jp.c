#include "fft/event_bunit.h"
#include "psx/types.h"

/* Menu row callback of BUNIT's ability list (bunit_ability_run_list_menu):
 * the JP that bunit_job_calculate_current_jp cached
 * for the job chosen in the job list. The row is not read. */
s32 bunit_job_get_current_jp(s32 unused_row) {
    return g_bunit_job_current_jp;
}
