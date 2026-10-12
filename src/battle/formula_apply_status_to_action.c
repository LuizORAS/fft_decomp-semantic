#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Apply the current ability's status set to the target action. Also the
 * handler of formula 0x38 (status, 100%).
 *
 * All-or-nothing copies the full set, random chooses one status, separate
 * rolls each status independently, and cancel writes the removal set. Separate
 * keeps a status when a roll of 0..99 is below 24, while the shown accuracy is
 * divided by four. Preview and AI evaluation force probabilistic modes through
 * the all-or-nothing path. A set that changes nothing on the target is a
 * forced miss. */
void formula_apply_status_to_action(void) {
    s32 status_ids[STATUS_COUNT];
    s32 type;
    s32 count;
    s32 status;
    s32 selected_status;
    s32 byte;
    s32 mask;
    u8 any;
    action_data_t* action;

    type = g_action_current_ability.status_infliction.type;
    if (type & STATUS_INFLICTION_TYPE_SEPARATE) {
        g_action_target_data->attack_accuracy /= 4;
    }
    if (type & (STATUS_INFLICTION_TYPE_SEPARATE | STATUS_INFLICTION_TYPE_RANDOM_ONE)) {
        if (g_action_state != ACTION_STATE_EXECUTE) {
            type = STATUS_INFLICTION_TYPE_ALL_OR_NOTHING;
        }
    }

    if (type & STATUS_INFLICTION_TYPE_ALL_OR_NOTHING) {
        status = 0;
        do {
            g_action_target_data->status_changes.infliction[status]
                |= g_action_current_ability.status_infliction.statuses[status];
            status++;
        } while (status < STATUS_BYTE_COUNT);
    } else if (type & STATUS_INFLICTION_TYPE_RANDOM_ONE) {
        count = 0;
        status = 0;
        do {
            if (g_action_current_ability.status_infliction.statuses[status / 8] & (0x80 >> (status & 7))) {
                status_ids[count] = status;
                count++;
            }
            status++;
        } while (status < STATUS_COUNT);

        selected_status = status_ids[(formula_get_random_0_7fff() * count) / 32768];
        g_action_target_data->status_changes.infliction[selected_status / 8] |= 0x80 >> (selected_status & 7);
    } else if (type & STATUS_INFLICTION_TYPE_SEPARATE) {
        status = 0;
        do {
            byte = status / 8;
            if (g_action_current_ability.status_infliction.statuses[byte] & (mask = 0x80 >> (status & 7))) {
                if (main_util_roll_pass_fail(100, 24) != 0 || g_action_state == ACTION_STATE_PREVIEW) {
                    g_action_current_ability.status_infliction.statuses[byte] ^= mask;
                }
            }
            status++;
        } while (status < STATUS_COUNT);

        status = 0;
        do {
            g_action_target_data->status_changes.infliction[status]
                |= g_action_current_ability.status_infliction.statuses[status];
            status++;
        } while (status < STATUS_BYTE_COUNT);
    } else if (type & STATUS_INFLICTION_TYPE_CANCEL) {
        status = 0;
        do {
            g_action_target_data->status_changes.removal[status]
                |= g_action_current_ability.status_infliction.statuses[status];
            status++;
        } while (status < STATUS_BYTE_COUNT);
    }

    any = 0;
    status = 0;
    action = g_action_target_data;
    do {
        any |= action->status_changes.infliction[status] | action->status_changes.removal[status];
        status++;
    } while (status < STATUS_BYTE_COUNT);

    if (any == 0 || status_modify_inflictions(0) == 0) {
        formula_force_attack_miss();
        return;
    }

    g_action_target_data->attack_type |= ACTION_TYPE_STATUS_CHANGE;
    if (g_action_target_data->status_changes.infliction[STATUS_BYTE_INDEX(STATUS_ID_INVITE)]
        & STATUS_BYTE_MASK(STATUS_ID_INVITE)) {
        g_action_target_data->special_effect |= ACTION_SPECIAL_EFFECT_TEAM_CHANGE;
    }
}
