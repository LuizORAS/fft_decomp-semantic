#include "fft/event_jobstts.h"
#include "fft/menu.h"
#include "psx/types.h"

s32 jobstts_job_get_current_level(s32 unused_row) {
    return g_menu_job_current_level;
}
