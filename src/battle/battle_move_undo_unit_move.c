#include "fft/battle_move.h"

/* Undo a move cancelled at the move confirmation (battle_state_handle_move_confirm_menu_state; a
 * teleport cannot be undone): release a mount the move climbed onto, return the unit to the square,
 * facing and mount pairing it had before the move (battle_move_return_to_previous_map_square), bring
 * back the rider of a mount, and pair a former rider with its mount again. */
void battle_move_undo_unit_move(battle_unit_t* unit) {
    battle_unit_t* other;

    if (unit->mount_state == BATTLE_UNIT_MOUNT_STATE_RIDER) {
        other = battle_unit_get_by_runtime_id(unit->mount_partner_runtime_id);
        other->mount_state = BATTLE_UNIT_MOUNT_STATE_NONE;
        battle_unit_update_display_by_runtime_id(other->runtime_id);
    }
    battle_move_return_to_previous_map_square(unit);
    if (unit->mount_state == BATTLE_UNIT_MOUNT_STATE_MOUNT) {
        other = battle_unit_get_by_runtime_id(unit->mount_partner_runtime_id);
        if (other != 0) {
            battle_move_return_to_previous_map_square(other);
            battle_unit_update_display_by_runtime_id(other->runtime_id);
        }
    } else if (unit->mount_state == BATTLE_UNIT_MOUNT_STATE_RIDER) {
        unit->status_flags_1_4 |= BATTLE_UNIT_STATUS_MOUNTED;
        other = battle_unit_get_by_runtime_id(unit->mount_partner_runtime_id);
        if (other != 0) {
            other->mount_state = BATTLE_UNIT_MOUNT_STATE_MOUNT;
            other->mount_partner_runtime_id = unit->runtime_id;
            battle_unit_update_display_by_runtime_id(other->runtime_id);
        }
    } else {
        unit->status_flags_1_4 &= ~BATTLE_UNIT_STATUS_MOUNTED;
    }
    battle_unit_update_display_by_runtime_id(unit->runtime_id);
}
