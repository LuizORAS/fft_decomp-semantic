#include "fft/world.h"

/* Return the lowest job id whose skillset is the given one, or -1. */
s32 world_job_find_first_for_skillset(s32 skillset) {
    s32 i;

    for (i = 0; i < JOB_ID_COUNT; i++) {
        if (main_job_get_data_pointer(i)->skillset == skillset) {
            return i;
        }
    }
    return -1;
}
