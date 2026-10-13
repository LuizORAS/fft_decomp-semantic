#include "fft/battle_move.h"

/* Cancel a move at the confirm prompt. The unit returns to its previous tile;
 * a mount takes its rider back with it, a unit that was riding is put back on
 * its mount, and a mount it climbed onto during the move is freed. */
void move_undo_unit_move(battle_unit_t* unit) {
    battle_unit_t* other;

    if (unit->mount_state == BATTLE_UNIT_MOUNT_STATE_RIDER) {
        other = battle_unit_get_by_runtime_id(unit->mount_partner_runtime_id);
        other->mount_state = BATTLE_UNIT_MOUNT_STATE_NONE;
        battle_unit_update_display_by_runtime_id(other->runtime_id);
    }
    move_return_to_previous_map_square(unit);
    if (unit->mount_state == BATTLE_UNIT_MOUNT_STATE_MOUNT) {
        other = battle_unit_get_by_runtime_id(unit->mount_partner_runtime_id);
        if (other != 0) {
            move_return_to_previous_map_square(other);
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
