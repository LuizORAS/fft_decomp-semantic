#include "fft/battle_action.h"
#include "fft/battle_event.h"
#include "psx/types.h"

/* Add total to the war funds, plus a quarter of the item's price when an item
 * is given (action_increment_item_quantity_for_steal_break passes one
 * the stock is full of). Only for a unit on team 0 and only when executing;
 * the funds stay within 0..99,999,999. Returns the amount added. */
s32 action_add_war_funds(battle_stats_t* unit, s32 total, u8 item_id) {
    s32 value;

    if (g_action_state != ACTION_STATE_EXECUTE) {
        return 0;
    }
    if (unit->team_flags & BATTLE_TEAM_MASK) {
        return 0;
    }
    if (item_id != ITEM_ID_NOTHING) {
        total += g_main_item_primary_data[item_id].price >> 2;
    }
    value = battle_script_get_variable(EVENT_SCRIPT_VAR_WAR_FUNDS) + total;
    if (value > 99999999) {
        value = 99999999;
    } else if (value < 0) {
        value = 0;
    }
    battle_script_set_variable(EVENT_SCRIPT_VAR_WAR_FUNDS, value);
    return total;
}
