#include "fft/battle_unit.h"
#include "fft/world.h"
#include "psx/types.h"

/* Filter a copied runtime unit ID by presence, team, and status for modes 0-5.
 *
 * Modes 3 and 5 reject the status check set 8; mode 5 ignores Undead. The
 * target has no defined return value for other modes; callers must pass 0-5.
 * `in_id` is read as a 16-bit value: callers pass the address of an int slot
 * counter or of an s16 slot-order entry. */
s32 world_script_filter_unit_id_by_mode(u16* out_id, void* in_id, s32* mode) {
    battle_stats_t* unit;
    s32 index;
    s32 i;
    s32 blocked;
    s32 query;
    u8 first;

    if (*mode == 0) {
        return 1;
    }
    *out_id = *(u16*)in_id;
    if (battle_unit_has_runtime_id(*out_id) != 0) {
        index = battle_unit_get_battle_id_by_runtime_id(*out_id);
        if (index != -1) {
            unit = battle_unit_get_stats_from_battle_id(index);
        }
    }
    if (*mode == 1) {
        return battle_unit_has_runtime_id(*out_id);
    }
    if (battle_unit_has_runtime_id(*out_id) == 0) {
        return 0;
    }
    query = *mode;
    if (query == 2) {
        if (index == -1) {
            return 1;
        }
        return (unit->team_flags & BATTLE_TEAM_MASK) == BATTLE_TEAM_BLUE;
    }
    if (query == 3) {
        if (index == -1) {
            return 1;
        }
        for (i = 0; i < STATUS_BYTE_COUNT; i++) {
            if ((unit->status_sets.current[i]
                    & g_main_status_check_sets[MAIN_STATUS_CHECK_SET_SCRIPT_FILTER_EXCLUDED][i])
                != 0) {
                break;
            }
        }
        if ((unit->team_flags & BATTLE_TEAM_MASK) == BATTLE_TEAM_BLUE && i == STATUS_BYTE_COUNT) {
            return 1;
        }
        return 0;
    }
    if (query == 4) {
        if (index == -1) {
            return 0;
        }
        return (unit->team_flags & BATTLE_TEAM_MASK) != BATTLE_TEAM_BLUE;
    }
    if (query == 5) {
        if (index == -1) {
            return 0;
        }
        first = g_main_status_check_sets[MAIN_STATUS_CHECK_SET_SCRIPT_FILTER_EXCLUDED][0];
        for (i = 0; i < STATUS_BYTE_COUNT; i++) {
            if (i == 0) {
                s32 status = unit->status_sets.current[0] & (0xff ^ STATUS_BYTE_MASK(STATUS_ID_UNDEAD));

                blocked = first & status;
            } else {
                blocked = unit->status_sets.current[i]
                    & g_main_status_check_sets[MAIN_STATUS_CHECK_SET_SCRIPT_FILTER_EXCLUDED][i];
            }
            if (blocked != 0) {
                break;
            }
        }
        if ((unit->team_flags & BATTLE_TEAM_MASK) != BATTLE_TEAM_BLUE && i == STATUS_BYTE_COUNT) {
            return 1;
        }
        return 0;
    }
}
