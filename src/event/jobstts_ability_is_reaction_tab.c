#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Reaction abilities occupy category one. */
s32 jobstts_ability_is_reaction_tab(s32 unused_row) {
    return g_jobstts_ability_category == MENU_ABILITY_CATEGORY_REACTION;
}
