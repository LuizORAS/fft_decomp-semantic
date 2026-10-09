#include "fft/event_jobstts.h"
#include "psx/types.h"

s32 jobstts_job_get_generic_index(s32 job_id) {
    u16 id = job_id;
    s16 result;

    if (id >= JOB_ID_SQUIRE && id <= JOB_ID_MIME) {
        result = id;
        result -= JOB_ID_SQUIRE;
    } else {
        result = 0;
    }
    return result;
}
