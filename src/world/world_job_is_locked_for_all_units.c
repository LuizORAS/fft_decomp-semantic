#include "fft/main.h"
#include "fft/world.h"

/* Returns 1 when no formation unit (special monsters aside) has generic job
 * `job_id` unlocked. */
s32 world_job_is_locked_for_all_units(s16 job_id) {
    s32 i;

    for (i = 0; i < g_world_formation_unit_count; i++) {
        if (job_is_special_monster(g_world_formation_unit_pointers[i]->identity.job_id) == 0) {
            world_bits_set_secondary_cursor(g_world_formation_unit_pointers[i]->unlocked_jobs);
            world_bits_read_secondary(job_id - JOB_ID_SQUIRE);
            if (world_bits_read_secondary(1) != 0) {
                return 0;
            }
        }
    }
    return 1;
}
