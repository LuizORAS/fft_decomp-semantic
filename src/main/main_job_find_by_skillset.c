#include "fft/main.h"
#include "psx/types.h"

/* Return the first job (below JOB_ID_COUNT - 1) whose job data names skillset_id, or 0 when none
 * does. Referenced nowhere on the disc; WORLD's job_find_first_for_skillset does the same job for the
 * formation's ability lists. */
s32 main_job_find_by_skillset(u8 skillset_id) {
    u8 job_id;
    s32 result;

    job_id = 0;
    for (; (result = job_id < JOB_ID_COUNT - 1) != 0; job_id++) {
        if (g_main_job_data_pointer[job_id].skillset == skillset_id) {
            return job_id;
        }
    }
    return result;
}
