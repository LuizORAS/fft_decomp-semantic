#include "fft/battle.h"

enum {
    POACH_ITEM_COMMON = 0,
    POACH_ITEM_RARE = 1,
};

/* While executing, give the Fur Shop the item of a poached monster (POACH special effect, monster jobs
 * Chocobo to Tiamat): the rare one with a chance of 31 in 256, else the common one
 * (g_main_item_poach_table). The stock saturates at 255. Returns the new stock, or 0 when nothing
 * was added.
 *
 * The first job read is volatile because the target performs eligibility and
 * table-index reads separately. */
s32 battle_action_add_poached_item_to_fur_shop_inventory(void) {
    battle_stats_t* unit;
    s32 idx;
    s32 item;
    s32 count;
    s32 result;
    s32 chance;

    chance = 0x100;
    if (g_battle_action_state != BATTLE_ACTION_STATE_EXECUTE) {
        return 0;
    }
    if ((g_battle_action_target_data->special_effect & BATTLE_ACTION_SPECIAL_EFFECT_POACH) == 0) {
        return 0;
    }
    unit = g_battle_action_target;
    if ((u32)(*(volatile u8*)&unit->job_id - JOB_ID_CHOCOBO) >= (JOB_ID_TIAMAT - JOB_ID_CHOCOBO + 1)) {
        return 0;
    }
    idx = unit->job_id - JOB_ID_CHOCOBO;
    if (main_util_roll_pass_fail(chance, 0x1f) != 0) {
        item = g_main_item_poach_table[idx][POACH_ITEM_COMMON];
    } else {
        item = g_main_item_poach_table[idx][POACH_ITEM_RARE];
    }
    count = g_main_item_poached_quantities[item];
    result = 0xff;
    if (count != result) {
        result = count + 1;
        g_main_item_poached_quantities[item] = result;
    }
    return result;
}
