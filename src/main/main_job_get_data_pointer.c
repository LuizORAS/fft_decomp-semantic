#include "fft/main.h"

/* Return job_id's record in g_main_job_data, or NULL past the table. */
job_data_t* main_job_get_data_pointer(s32 job_id) {
    if (job_id < JOB_ID_COUNT) {
        return &g_main_job_data[job_id];
    }
    return 0;
}
