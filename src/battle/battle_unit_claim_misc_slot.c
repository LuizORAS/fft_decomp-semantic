#include "fft/battle_unit.h"

/*
 * Claim a Misc Unit Data slot and stamp its unit id.
 *
 * Ids below 16 take their own slot, failing if another in-use slot already
 * carries that id or the slot is taken. Larger ids take the first free slot,
 * whose index becomes the stored id; exhausting all 16 raises animation error
 * 6. Returns 0 on failure.
 */
battle_unit_t* battle_unit_claim_misc_slot(s16 unit_id) {
    s32 i;

    if (unit_id < BATTLE_UNIT_RUNTIME_SLOT_COUNT) {
        for (i = 0; i < BATTLE_UNIT_RUNTIME_SLOT_COUNT; i++) {
            if (g_battle_unit_slot_flags[i].in_use != 0 && g_battle_units[i].runtime_id == unit_id) {
                break;
            }
        }
        if (i < BATTLE_UNIT_RUNTIME_SLOT_COUNT) {
            return 0;
        }
        if (g_battle_unit_slot_flags[unit_id].in_use != 0) {
            return 0;
        }
        g_battle_unit_slot_flags[unit_id].in_use = 1;
        g_battle_units[unit_id].runtime_id = unit_id;
        return &g_battle_units[unit_id];
    }
    for (i = 0; i < BATTLE_UNIT_RUNTIME_SLOT_COUNT; i++) {
        if (g_battle_unit_slot_flags[i].in_use == 0) {
            g_battle_unit_slot_flags[i].in_use = 1;
            g_battle_units[i].runtime_id = i;
            return &g_battle_units[i];
        }
    }
    main_system_handle_error(MAIN_SYSTEM_ERROR_NO_UNIT_RUNTIME_SLOT);
    return 0;
}
