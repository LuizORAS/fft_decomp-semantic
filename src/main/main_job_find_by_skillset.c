#include "fft/battle.h"
#include "fft/main.h"
#include "psx/types.h"

/* Return the first job (below JOB_ID_COUNT - 1) whose job data names skillset_id, or 0 when none
 * does. Referenced nowhere on the disc; WORLD's world_job_find_first_for_skillset does the same job for the
 * formation's ability lists. */
s32 main_job_find_by_skillset(s32 skillset_id) {
    u32 job_id;
    s32 result;
    job_data_t* job_data;

    job_id = 0;
    job_data = g_main_job_data_pointer;
    skillset_id &= 0xff;

    do {
        s32 checked_job_id = job_id & 0xff;

        if (job_data[checked_job_id].skillset == skillset_id) {
            result = checked_job_id;
            break;
        }
        job_id++;
        result = (job_id & 0xff) < (JOB_ID_COUNT - 1);
    } while (result != 0);
    return result;
}
