#include "fft/battle.h"
#include "psx/types.h"

/* Item a fallen unit leaves as treasure: one of its equipped items at random,
 * or with nothing equipped one of the thirteen consumables other than Elixir.
 * Decided only when executing; in any other state the function returns without
 * a value. */
u8 battle_unit_generate_treasure(battle_stats_t* unit) {
    u8 buf[8];
    s32 count;
    s32 i;
    u32 item_id;

    if (g_battle_action_state != BATTLE_ACTION_STATE_EXECUTE) {
        return;
    }

    count = 0;
    for (i = 0; i < 7; i++) {
        buf[count] = unit->equipment[i];
        item_id = buf[count];
        if (item_id >= ITEM_ID_END) {
            continue;
        }
        if (item_id == 0) {
            continue;
        }
        count++;
    }

    if (count == 0) {
        buf[0] = ((rand() * 13) / 0x8000) - (ITEM_ID_COUNT - ITEM_ID_CONSUMABLE_FIRST);
        if (buf[0] >= ITEM_ID_ELIXIR) {
            buf[0] = buf[0] + 1;
        }
        count = 1;
    }

    i = (rand() * count) / 0x8000;
    return buf[i];
}
