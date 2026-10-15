#include "fft/wldcore.h"
#include "psx/types.h"

/* Proposition job category of a job: 0..19 for Squire..Mime, 22 for the
 * special jobs below Squire and 21 for anything above Mime (monsters). */
s32 wldcore_proposition_get_job_category(s32 job_id) {
    if (job_id < JOB_ID_SQUIRE) {
        return 22;
    }
    if (job_id <= JOB_ID_MIME) {
        return job_id - JOB_ID_SQUIRE;
    }
    return 21;
}
