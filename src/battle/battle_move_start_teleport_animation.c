#include "fft/battle_move.h"

/* Starts the teleport animation and sets the step destination: the tile in the path bytes,
 * or the unit's own tile when the teleport failed. */
void battle_move_start_teleport_animation(battle_unit_t* unit) {
    unit->step_phase = BATTLE_MOVE_STEP_PHASE_TELEPORT_ANIMATION;
    battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_TELEPORT, unit->facing, unit);
    if (unit->path.count == BATTLE_MOVE_PATH_TELEPORT) {
        unit->movement.destination_x = unit->path.data.teleport.x;
        unit->movement.destination_y = unit->path.data.teleport.y;
        unit->movement.destination_z = unit->path.data.teleport.elevation;
    } else {
        unit->movement.destination_x = unit->map_x;
        unit->movement.destination_y = unit->map_y;
        unit->movement.destination_z = unit->map_z;
    }
}
