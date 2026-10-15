#include "fft/world.h"
#include "psx/types.h"

/* Menu row callback of WORLD's ability list (world_ability_run_learn_list, world_ability_run_view_list):
 * the mastered flag that world_job_calculate_current_mastered cached
 * for the job chosen in the job list. The row is not read. */
s32 world_job_get_cached_skillset_empty_flag(s32 unused_row) {
    return g_world_job_cached_skillset_empty;
}
