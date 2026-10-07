#include "fft/battle.h"

/* Put a unit back where it was before its move: map square, facing and mount pairing from the
 * previous_* fields (saved by battle_unit_save_previous_state), then its screen position, its battle
 * record's tile and its current animation at that facing. */
void battle_move_return_to_previous_map_square(battle_unit_misc_data_t* unit) {
    unit->map_x = unit->previous_map_x;
    unit->map_y = unit->previous_map_y;
    unit->map_z = unit->previous_map_z;
    unit->facing = unit->previous_facing;
    unit->mount_state = unit->previous_mount_state;
    unit->mount_partner_misc_id = unit->previous_mount_partner_misc_id;
    unit->current_unit_id_plus_one = unit->previous_unit_id_plus_one;
    battle_unit_set_move_and_screen_coords(unit);
    battle_unit_set_tile_position(
        unit->battle_data->misc_unit_id, unit->map_x, unit->map_y, unit->map_z, (u8)(unit->facing / 0x400));
    battle_unit_store_animation_facing_movement_data((u32)unit->encoded_animation >> 1, unit->facing, (u8*)unit);
}
