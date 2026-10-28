#include "fft/world.h"
#include "psx/types.h"

/* Menu row callback of WORLD's ability list (world_ability_run_learn_list, world_ability_run_view_list):
 * the job level that world_job_calculate_current_level cached
 * for the job chosen in the job list. The row is not read. */
s32 world_job_get_cached_level(s32 unused_row) {
    return g_world_job_cached_level;
}
