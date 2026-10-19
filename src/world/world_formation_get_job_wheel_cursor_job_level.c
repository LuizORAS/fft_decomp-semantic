#include "fft/main.h"
#include "fft/world.h"
#include "psx/types.h"

/* Job level of the job under the job-wheel cursor (menu script callback 0 of the wheel); job_levels
 * packs two generic jobs per byte, the even index in the high nibble. */
s32 world_formation_get_job_wheel_cursor_job_level(s32 unused_row) {
    s32 index = job_get_generic_index(g_world_job_wheel_jobs[g_world_job_wheel_cursor_index]);
    s32 level = g_world_formation_unit_pointers[g_world_formation_selected_unit_index]->job_levels[index >> 1];

    if ((index & 1) == 0) {
        level >>= 4;
    } else {
        level &= 0xf;
    }
    return level;
}
