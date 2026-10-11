#include "fft/event_bunit.h"
#include "psx/types.h"

/* The skillset of job job_id. */
s32 bunit_job_get_skillset(s32 job_id) {
    return main_job_get_data_pointer(job_id)->skillset;
}
