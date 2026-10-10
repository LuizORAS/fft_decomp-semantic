#include "fft/world.h"
#include "psx/types.h"

s32 world_formation_is_job_wheel_selection_available(s32 unused_row) {
    if (g_world_job_wheel_job_count < 2) {
        return 0;
    }
    return (g_world_job_wheel_jobs[g_world_job_wheel_cursor_index] & WORLD_JOB_LIST_ENTRY_UNAVAILABLE) == 0;
}
