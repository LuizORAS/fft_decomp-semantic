#include "fft/battle.h"

/* Stage the graphics of a status change on the unit's misc record: status is the handler index
 * (status ID + 1), enabled 1 adds and 0 removes. Crystal, Chicken, Frog, Treasure, Jump, Float and
 * Transparent use statuses_to_add_5_6 and statuses_to_remove_5_6, the others the 1_4 pair through
 * g_battle_misc_status_mask_by_handler_index; status 0, Invite and Wall have no graphics, and
 * Treasure none on removal. Removing Crystal removes the Chicken graphic. A Frog change also rebuilds
 * the unit's AI ability list (battle_ai_init_unit_abilities). Returns 0 for a unit with no misc
 * record, else 1. */
s32 battle_status_queue_misc_graphics_flag_change(s32 status, s32 enabled, s32 battle_id) {
    battle_unit_misc_data_t* unit;

    unit = battle_unit_get_misc_data_by_battle_id(battle_id & 0xffff);
    if (unit == 0) {
        return 0;
    }
    switch (enabled) {
    case 0:
        switch (status) {
        case BATTLE_STATUS_HANDLER_INDEX(BATTLE_STATUS_ID_CRYSTAL):
        case BATTLE_STATUS_HANDLER_INDEX(BATTLE_STATUS_ID_CHICKEN):
            unit->statuses_to_remove_5_6 |= BATTLE_MISC_STATUS_CHICKEN;
            unit->statuses_to_add_5_6 &= ~BATTLE_MISC_STATUS_CHICKEN;
            break;
        case BATTLE_STATUS_HANDLER_INDEX(BATTLE_STATUS_ID_FROG):
            unit->statuses_to_remove_5_6 |= BATTLE_MISC_STATUS_FROG;
            unit->statuses_to_add_5_6 &= ~BATTLE_MISC_STATUS_FROG;
            battle_ai_init_unit_abilities(battle_id);
            return 1;
        case BATTLE_STATUS_HANDLER_INDEX(BATTLE_STATUS_ID_JUMP):
        case BATTLE_STATUS_HANDLER_INDEX(BATTLE_STATUS_ID_FLOAT):
        case BATTLE_STATUS_HANDLER_INDEX(BATTLE_STATUS_ID_TRANSPARENT):
            unit->statuses_to_remove_5_6 |= g_battle_misc_status_mask_by_handler_index[status];
            unit->statuses_to_add_5_6 &= ~g_battle_misc_status_mask_by_handler_index[status];
            break;
        case BATTLE_STATUS_HANDLER_INDEX(BATTLE_STATUS_ID_UNNAMED_00):
        case BATTLE_STATUS_HANDLER_INDEX(BATTLE_STATUS_ID_INVITE):
        case BATTLE_STATUS_HANDLER_INDEX(BATTLE_STATUS_ID_TREASURE):
        case BATTLE_STATUS_HANDLER_INDEX(BATTLE_STATUS_ID_WALL):
            break;
        default:
            unit->statuses_to_remove_1_4 |= g_battle_misc_status_mask_by_handler_index[status];
            unit->statuses_to_add_1_4 &= ~g_battle_misc_status_mask_by_handler_index[status];
            break;
        }
        break;
    case 1:
        switch (status) {
        case BATTLE_STATUS_HANDLER_INDEX(BATTLE_STATUS_ID_CRYSTAL):
            unit->statuses_to_add_5_6 |= BATTLE_MISC_STATUS_CRYSTAL;
            unit->statuses_to_remove_5_6 &= ~BATTLE_MISC_STATUS_CRYSTAL;
            break;
        case BATTLE_STATUS_HANDLER_INDEX(BATTLE_STATUS_ID_CHICKEN):
            unit->statuses_to_add_5_6 |= BATTLE_MISC_STATUS_CHICKEN;
            unit->statuses_to_remove_5_6 &= ~BATTLE_MISC_STATUS_CHICKEN;
            break;
        case BATTLE_STATUS_HANDLER_INDEX(BATTLE_STATUS_ID_FROG):
            unit->statuses_to_add_5_6 |= BATTLE_MISC_STATUS_FROG;
            unit->statuses_to_remove_5_6 &= ~BATTLE_MISC_STATUS_FROG;
            battle_ai_init_unit_abilities(battle_id);
            return 1;
        case BATTLE_STATUS_HANDLER_INDEX(BATTLE_STATUS_ID_TREASURE):
            unit->statuses_to_add_5_6 |= BATTLE_MISC_STATUS_TREASURE;
            unit->statuses_to_remove_5_6 &= ~BATTLE_MISC_STATUS_TREASURE;
            break;
        case BATTLE_STATUS_HANDLER_INDEX(BATTLE_STATUS_ID_JUMP):
        case BATTLE_STATUS_HANDLER_INDEX(BATTLE_STATUS_ID_FLOAT):
        case BATTLE_STATUS_HANDLER_INDEX(BATTLE_STATUS_ID_TRANSPARENT):
            unit->statuses_to_add_5_6 |= g_battle_misc_status_mask_by_handler_index[status];
            unit->statuses_to_remove_5_6 &= ~g_battle_misc_status_mask_by_handler_index[status];
            break;
        case BATTLE_STATUS_HANDLER_INDEX(BATTLE_STATUS_ID_UNNAMED_00):
        case BATTLE_STATUS_HANDLER_INDEX(BATTLE_STATUS_ID_INVITE):
        case BATTLE_STATUS_HANDLER_INDEX(BATTLE_STATUS_ID_WALL):
            break;
        default:
            unit->statuses_to_add_1_4 |= g_battle_misc_status_mask_by_handler_index[status];
            unit->statuses_to_remove_1_4 &= ~g_battle_misc_status_mask_by_handler_index[status];
            break;
        }
        break;
    case 2:
        /* No effect, but the label makes 1 the root of the target's
         * compare tree. */
        break;
    }
    return 1;
}
