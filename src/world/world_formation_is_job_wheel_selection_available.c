#include "fft/world.h"
#include "psx/types.h"

/* Menu callback 1 of the job wheel: 1 when the job under the wheel cursor can be chosen; 0 for an
 * unavailable job or a wheel with fewer than two jobs. */
s32 world_formation_is_job_wheel_selection_available(s32 unused_row) {
    if (g_world_job_wheel_job_count < 2) {
        return 0;
    }
    return (g_world_job_wheel_jobs[g_world_job_wheel_cursor_index] & WORLD_JOB_LIST_ENTRY_UNAVAILABLE) == 0;
}
