#include "fft/battle_move.h"
#include "psx/types.h"

/* Path a unit to a destination tile and start it walking at the given speed. */
s32 battle_move_start_unit_walk_to(
    s32 runtime_id, map_tile_coordinates_t* destination, s32 flags, s32 jump, s32 speed) {
    battle_unit_t* unit;
    battle_walk_path_t* path;

    unit = battle_unit_get_by_runtime_id(runtime_id & 0xFFFF);
    if (unit != 0) {
        path = battle_move_calculate_walkto_pathing(
            flags, jump, unit->map_x, unit->map_y, unit->map_z, destination->x, destination->y, destination->layer);
        if (path != 0) {
            unit->path = *path;
            unit->walk_speed = speed * 2;
            unit->step_speed = 0x2000;
            unit->movement_path_offset = 0;
            battle_unit_dismount_rider_and_update_display(unit);
            return 1;
        }
    } else {
        main_system_handle_pointer_exception(0xC);
    }
    return 0;
}
