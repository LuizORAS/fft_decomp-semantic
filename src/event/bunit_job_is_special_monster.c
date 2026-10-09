#include "fft/event_bunit.h"
#include "psx/types.h"

s32 bunit_job_is_special_monster(s32 job_id) {
    s32 result;

    result = job_id >= JOB_ID_SPECIAL_MONSTER_1_FIRST && job_id < JOB_ID_GENERIC_FIRST;
    if (job_id >= JOB_ID_SPECIAL_MONSTER_2_FIRST
        && job_id < JOB_ID_SPECIAL_MONSTER_2_FIRST + JOB_ID_SPECIAL_MONSTER_2_COUNT) {
        result = 1;
    }
    return result;
}
