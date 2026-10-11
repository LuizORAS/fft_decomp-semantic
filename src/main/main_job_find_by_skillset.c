#include "fft/main.h"
#include "psx/types.h"

/* Return the first job id below JOB_ID_COUNT - 1 whose job record has this
 * skillset. The result is 0 when none has, which is also job 0's id. */
s32 main_job_find_by_skillset(u8 skillset_id) {
    u8 job_id;
    s32 result;
    job_data_t* job_data;

    job_id = 0;
    job_data = g_main_job_data_pointer;

    do {
        u8 checked_job_id = job_id;

        if (job_data[checked_job_id].skillset == skillset_id) {
            result = checked_job_id;
            break;
        }
        job_id++;
        result = job_id < (JOB_ID_COUNT - 1);
    } while (result != 0);
    return result;
}
