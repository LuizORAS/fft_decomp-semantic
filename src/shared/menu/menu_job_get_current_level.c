#include "fft/menu.h"

/* Menu row callback of the ability list (bunit_ability_run_list_menu in BUNIT,
 * jobstts_ability_run_list_menu in JOBSTTS): the job level that the overlay's
 * job_calculate_current_level cached for the job chosen in the job list. The row is not read. */
s32 menu_job_get_current_level(s32 unused_row) {
    return g_menu_job_current_level;
}
