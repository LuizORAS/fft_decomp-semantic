#include "fft/battle.h"
#include "psx/types.h"

/* The potion Auto Potion uses: a unit that is not blue always has a Potion; a blue one takes the first
 * the party has of Potion, Hi-Potion and X-Potion, or -1 with none. */
s32 battle_reaction_select_auto_potion_item(battle_stats_t* unit) {
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
