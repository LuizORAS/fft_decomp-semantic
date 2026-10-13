#include "fft/battle_action.h"
#include "psx/types.h"

/* The item Auto Potion uses: the first of Potion, Hi-Potion and X-Potion the
 * party has in stock, or -1 with none. A unit that did not start on the blue team
 * always uses a Potion. */
s32 action_select_auto_potion_item(unit_stats_t* unit) {
    if (unit->initial_team_flags & BATTLE_TEAM_MASK) {
        return ITEM_ID_POTION;
    }
    if (g_main_item_quantities[ITEM_ID_POTION] != 0) {
        return ITEM_ID_POTION;
    }
    if (g_main_item_quantities[ITEM_ID_HI_POTION] != 0) {
        return ITEM_ID_HI_POTION;
    }
    if (g_main_item_quantities[ITEM_ID_X_POTION] != 0) {
        return ITEM_ID_X_POTION;
    }
    return -1;
}
