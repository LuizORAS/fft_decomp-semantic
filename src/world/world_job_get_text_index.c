#include "fft/world.h"
#include "psx/types.h"

s32 world_job_get_text_index(s32 job_id) {
    if ((s16)job_id < JOB_ID_SQUIRE) {
        return (s16)(job_id + 19);
    }
    return (s16)(job_id - JOB_ID_SQUIRE);
}
