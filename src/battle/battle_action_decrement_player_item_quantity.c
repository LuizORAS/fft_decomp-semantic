#include "fft/battle.h"
#include "psx/types.h"

/* Use one of an item from the party inventory. Only blue units draw on it: -1 when the party has
 * none, and with consume set during an executing action (not an AI simulation or preview) one is
 * taken. Returns 0 otherwise, also for other teams, whose items are not counted.
 * battle_menu_collect_throwable_items passes consume 0 to test what can be thrown. */
s32 battle_action_decrement_player_item_quantity(battle_stats_t* unit, s32 item_id, s32 consume) {
    u32 idx;
    s32 qty;

    if ((unit->initial_team_flags & BATTLE_TEAM_MASK) == BATTLE_TEAM_BLUE) {
        idx = item_id & 0xFF;
        qty = g_main_item_quantities[idx];
        if (qty == 0) {
            return -1;
        }
        if (consume == 0) {
            return 0;
        }
        if (g_battle_action_state != BATTLE_ACTION_STATE_EXECUTE) {
            return 0;
        }
        g_main_item_quantities[idx] = qty - 1;
    }
    return 0;
}
