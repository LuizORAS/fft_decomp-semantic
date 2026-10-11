#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Movement abilities occupy category three. */
s32 jobstts_ability_is_movement_tab(s32 unused_row) {
    return g_jobstts_ability_category == MENU_ABILITY_CATEGORY_MOVEMENT;
}
