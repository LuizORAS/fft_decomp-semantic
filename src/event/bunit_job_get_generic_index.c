#include "fft/event_bunit.h"
#include "psx/types.h"

/* Map generic jobs (Squire through Mime) to zero-based indexes; unique jobs map to 0. */
s32 bunit_job_get_generic_index(s32 job_id) {
    u16 id = job_id;
    s16 result;

    if ((u16)(id - JOB_ID_SQUIRE) < JOB_ID_GENERIC_COUNT) {
        result = id;
        result -= JOB_ID_SQUIRE;
    } else {
        result = 0;
    }
    return result;
}
