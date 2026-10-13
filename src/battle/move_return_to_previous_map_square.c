#include "fft/battle_move.h"

/* Put the unit back on the tile, facing and mount state saved before its move,
 * then refresh its screen position and animation facing. */
void move_return_to_previous_map_square(battle_unit_t* unit) {
    unit->map_x = unit->previous_map_x;
    unit->map_y = unit->previous_map_y;
    unit->map_z = unit->previous_map_z;
    unit->facing = unit->previous_facing;
    unit->mount_state = unit->previous_mount_state;
    unit->mount_partner_runtime_id = unit->previous_mount_partner_runtime_id;
    unit->standing_on_battle_id_plus_one = unit->previous_standing_on_battle_id_plus_one;
    battle_unit_set_move_and_world_coords(unit);
    battle_unit_set_tile_position(unit->battle_data->battle_id, unit->map_x, unit->map_y, unit->map_z,
        (unit->facing / BATTLE_FACING_QUARTER_TURN));
    battle_unit_store_animation_facing_movement_data(unit->animation.sequence_id >> 1, unit->facing, unit);
}
