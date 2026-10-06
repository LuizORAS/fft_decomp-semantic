#include "fft/battle.h"
#include "psx/types.h"

/* Add total gil to the war funds (script variable EVENT_SCRIPT_VAR_WAR_FUNDS, kept in
 * 0..99,999,999) while executing, for a blue unit only; an item id other than ITEM_ID_NOTHING adds its
 * price / 4. Returns the amount added. */
s32 battle_action_add_war_funds(battle_stats_t* unit, s32 total, u8 item_id) {
    s32 value;

    if (g_battle_action_state != BATTLE_ACTION_STATE_EXECUTE) {
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
