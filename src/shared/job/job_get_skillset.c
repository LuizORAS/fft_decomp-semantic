#include "fft/main.h"

/* Return job `job_id`'s skillset, from MAIN's job data. */
s32 job_get_skillset(s32 job_id) {
    return main_job_get_data_pointer(job_id)->skillset;
}
