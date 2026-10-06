#include "fft/battle.h"
#include "psx/types.h"

/* Add one of an item to the party inventory for a blue unit while executing: a stolen item, a caught
 * one (Catch), a Draw Out katana that did not break, or a picked-up treasure's item. Nothing and an empty
 * slot are skipped. With 99 already owned (equipped ones counted), the item becomes its price / 4 in
 * war funds instead. */
s32 battle_action_add_party_item(battle_stats_t* unit, s32 item) {
    s32 item_id;

    if (g_battle_action_state != BATTLE_ACTION_STATE_EXECUTE) {
        return 0;
    }
    if ((unit->initial_team_flags & BATTLE_TEAM_MASK) != 0) {
        return 0;
    }
    item_id = item & 0xFF;
    if (item_id == ITEM_ID_NOTHING) {
        return 0;
    }
    if (item_id == ITEM_ID_NONE) {
        return 0;
    }
    if (get_total_equipment_quantity(item_id, 1) >= 0x63) {
        return battle_action_add_war_funds(unit, 0, item_id);
    }
    g_main_item_quantities[item_id] += 1;
    return 0;
}
