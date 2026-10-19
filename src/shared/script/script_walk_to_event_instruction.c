#include "fft/battle_move.h"
#include "fft/script.h"

/* WalkTo (EVENT_OPCODE_WALK_TO): start the unit walking to tile x, y on the given elevation at the
 * given speed (battle_move_start_unit_walk_to); a unit id that does not resolve does nothing. With
 * use_terrain_costs clear the path costs 1 on every tile (pathing flag 0x100); set, it takes the
 * terrain's costs.
 *
 * Coordinates are ordered x, elevation, y for the movement subsystem. */
void script_walk_to_event_instruction(const event_walk_to_parameters_t* parameters) {
    s32 id;
    map_tile_coordinates_t coordinates;

    id = script_load_halfword(parameters->unit_id_le);
    id = script_resolve_runtime_id(id);
    if (id != EVENT_RUNTIME_ID_NONE) {
        coordinates.x = parameters->x;
        coordinates.y = parameters->y;
        coordinates.layer = parameters->elevation;
        battle_move_start_unit_walk_to(id, &coordinates,
            BATTLE_MOVE_PATHFIND_IGNORE_TERRAIN_COST
                - parameters->use_terrain_costs * BATTLE_MOVE_PATHFIND_IGNORE_TERRAIN_COST,
            3, script_load_halfword(parameters->speed_le));
    }
}
