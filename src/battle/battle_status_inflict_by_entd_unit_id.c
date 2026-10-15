#include "fft/battle.h"
#include "psx/types.h"

/* The event's Inflict Status: on the unit with ENTD id entd_id (none found: nothing), stage one
 * status, numbered from the low bit of its byte, as an infliction (which nonzero) or a removal, and
 * resolve it on the main stack (battle_status_resolve_unit_changes with removal_only). */
void battle_status_inflict_by_entd_unit_id(s32 entd_id, s32 status, s32 which, s32 removal_only) {
    s32 idx;
    battle_stats_t* unit;
    s32 i;
    s32 byte;
    s32 bit;

    idx = battle_unit_get_battle_index_by_entd_unit_id(entd_id);
    if (idx == 0x7D0) {
        return;
    }
    unit = battle_unit_get_stats_from_battle_id(idx);
    for (i = 0; i < BATTLE_STATUS_BYTE_COUNT; i++) {
        unit->action.status_infliction[i] = 0;
        unit->action.status_removal[i] = 0;
    }
    /* The event numbers a status from the low bit of its byte; BATTLE_STATUS_ID and the action's
     * bytes count from the high bit (BATTLE_STATUS_BYTE_MASK). */
    byte = status / 8;
    bit = 1 << (status - byte * 8);
    if (which != 0) {
        unit->action.status_infliction[byte] = bit;
    } else {
        unit->action.status_removal[byte] = bit;
    }
    g_battle_thread_call_target = (void (*)(void))battle_status_resolve_unit_changes;
    battle_thread_call_on_main_stack(idx, removal_only);
}
