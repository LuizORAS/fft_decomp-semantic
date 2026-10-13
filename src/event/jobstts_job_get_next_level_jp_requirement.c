#include "fft/event_jobstts.h"
#include "fft/menu.h"
#include "psx/types.h"

s32 jobstts_job_get_next_level_jp_requirement(s32 unused_row) {
    return g_menu_job_next_level_jp_requirement;
}
