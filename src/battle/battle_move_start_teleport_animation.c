#include "fft/battle.h"
#include "psx/types.h"

/* Starts the teleport animation and sets the step destination: the tile in the path bytes,
 * or the unit's own tile when the teleport failed. */
void battle_move_start_teleport_animation(battle_unit_misc_data_t* unit) {
    unit->step_phase = BATTLE_MOVE_STEP_PHASE_TELEPORT_ANIMATION;
    battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_TELEPORT, unit->facing, unit);
    if (unit->movement_path_count == BATTLE_MOVE_PATH_TELEPORT) {
        unit->movement.bytes.destination_x = unit->movement_path[0].value;
        unit->movement.bytes.destination_y = unit->movement_path[1].value;
        unit->movement.bytes.destination_z = unit->movement_path[2].value;
    } else {
        unit->movement.bytes.destination_x = unit->map_x;
        unit->movement.bytes.destination_y = unit->map_y;
        unit->movement.bytes.destination_z = unit->map_z;
    }
}
