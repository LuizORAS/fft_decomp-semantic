#include "fft/event_jobstts.h"
#include "psx/types.h"

s32 jobstts_job_get_next_level_jp_requirement(s32 unused_row) {
    return g_jobstts_job_next_level_jp_requirement;
}
