#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Support abilities occupy category two. */
s32 jobstts_ability_is_support_tab(s32 unused_row) {
    return g_jobstts_ability_category == MENU_ABILITY_CATEGORY_SUPPORT;
}
