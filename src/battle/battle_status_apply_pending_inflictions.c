#include "fft/battle.h"
#include "psx/types.h"

/* Apply the inflictions in unit_id's action record, status by status. A status that one of the
 * unit's inflicted statuses blocks (cant_stack) is skipped; otherwise its cancel list becomes the
 * removal set, the changes are filtered and the removals applied, the status is inflicted and its
 * count started (a Death Sentence already counting stops there), and its graphics queued (2 when
 * re-inflicting anything but Crystal or Oil). Transparent clears its removal flag; Charm and Invite
 * give the attacker's team, Invite for good (initial team, no Auto-Battle). A Crystal or Dead unit
 * of the player's team sets g_battle_player_unit_fallen. Ends by rebuilding the current set. */
void battle_status_apply_pending_inflictions(s32 unit_id, s32 removal_only) {
    s32 i;
    s32 j;
    s32 byte;
    s32 mask;
    s32 bit;
    s32 blocked;
    u8 previous;

    g_current_ability.target_id = unit_id;
    g_battle_action_target = &g_battle_unit_stats[unit_id];
    g_battle_action_target_data = &g_battle_unit_stats[unit_id].action;
    for (i = 0; i < BATTLE_STATUS_COUNT; i++) {
        byte = i / 8;
        /* The helper's u8 cast adds a mask instruction for a dynamic ID. */
        bit = 0x80 >> (i & 7);
        /* Keeps the srav ahead of the infliction load, not in its delay slot. */
        __asm__("" : : "r"(bit));
        if (!(g_battle_action_target_data->status_infliction[byte] & bit)) {
            continue;
        }
        mask = bit;
        blocked = 0;
        for (j = 0; j < BATTLE_STATUS_BYTE_COUNT; j++) {
            if (g_battle_action_target->inflicted_status[j] & g_main_status_effect_data[i].cant_stack[j]) {
                blocked = 1;
                break;
            }
        }
        if (blocked != 0) {
            continue;
        }
        for (j = 0; j < BATTLE_STATUS_BYTE_COUNT; j++) {
            g_battle_action_target_data->status_removal[j] = g_main_status_effect_data[i].cancels[j];
        }
        battle_status_modify_inflictions(removal_only);
        battle_status_apply_pending_removals(unit_id);
        previous = g_battle_action_target->inflicted_status[byte];
        g_battle_action_target->inflicted_status[byte] = previous | mask;
        if (main_status_set_ct(g_battle_action_target, i, 0) != 0) {
            continue;
        }
        if ((previous & mask) && i != BATTLE_STATUS_ID_CRYSTAL && i != BATTLE_STATUS_ID_OIL) {
            battle_status_queue_graphics_change_if_executing(BATTLE_STATUS_HANDLER_INDEX(i), 2, unit_id);
        } else {
            battle_status_queue_graphics_change_if_executing(BATTLE_STATUS_HANDLER_INDEX(i), 1, unit_id);
        }
        if (i == BATTLE_STATUS_ID_TRANSPARENT) {
            g_battle_action_target->transparent_removal_flag = 0;
        }
        if (i == BATTLE_STATUS_ID_INVITE || i == BATTLE_STATUS_ID_CHARM) {
            previous = (g_battle_action_target->team_flags & 0xcf)
                | (g_battle_action_attacker->team_flags & BATTLE_TEAM_MASK);
            g_battle_action_target->team_flags = previous;
            if (i == BATTLE_STATUS_ID_INVITE) {
                previous = (g_battle_action_target->initial_team_flags & 0xc7)
                    | (g_battle_action_attacker->team_flags & BATTLE_TEAM_MASK);
                g_battle_action_target_data->reaction_id = 0;
                g_battle_action_target->initial_team_flags = previous;
                g_battle_action_target->auto_battle_setting = 0;
                g_battle_action_target->auto_battle_target = 0;
            }
        }
        if (!(g_battle_action_target->initial_team_flags & BATTLE_TEAM_MASK)
            && (u32)(i - BATTLE_STATUS_ID_CRYSTAL) < 2) {
            g_battle_player_unit_fallen = 1;
        }
    }
    main_status_store_current(g_battle_action_target);
}
