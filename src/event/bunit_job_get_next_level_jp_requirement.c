#include "fft/event_bunit.h"
#include "fft/menu.h"
#include "psx/types.h"

s32 bunit_job_get_next_level_jp_requirement(s32 unused_row) {
    return g_menu_job_next_level_jp_requirement;
}
