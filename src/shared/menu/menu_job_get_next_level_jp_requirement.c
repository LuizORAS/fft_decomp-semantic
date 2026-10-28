#include "fft/menu.h"

/* Menu row callback of the ability list (bunit_ability_run_list_menu in BUNIT,
 * jobstts_ability_run_list_menu in JOBSTTS): the next level's JP that
 * menu_job_calculate_next_level_jp_requirement cached for the job chosen in the job list. The row is
 * not read. */
s32 menu_job_get_next_level_jp_requirement(s32 unused_row) {
    return g_menu_job_next_level_jp_requirement;
}
