#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Row condition: 1 while the ability list shows the action tab (MENU_ABILITY_CATEGORY_ACTION). */
s32 jobstts_ability_is_action_tab(s32 unused_row) {
    return g_jobstts_ability_category == MENU_ABILITY_CATEGORY_ACTION;
}
