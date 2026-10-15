#include "fft/world.h"

/* Builds the unit's known skillset list.
 *
 * Excludes its current job and Mime, and terminates the result with -1. */
s32 world_ability_list_secondary_skillsets(s16 unit_id, s16* skillsets) {
    s16 jobs[24];
    s32 count;
    s32 i;
    s32 job;
    s16 own_job;

    own_job = g_world_formation_unit_pointers[unit_id]->job_id;
    world_job_build_unit_list(unit_id, jobs, 1);
    count = 0;
    for (i = 0; jobs[i] != -1; i++) {
        job = jobs[i];
        if (own_job != job && job != JOB_ID_MIME) {
            skillsets[count++] = world_job_get_skillset(job);
        }
    }
    skillsets[count] = -1;
    return count;
}
