#include "fft/battle.h"

/* WalkTo (EVENT_OPCODE_WALK_TO): start the unit walking to tile x, y on the given elevation at the
 * given speed (battle_move_start_unit_walk_to); a unit id that does not resolve does nothing. With
 * use_terrain_costs clear the path costs 1 on every tile (pathing flag 0x100); set, it takes the
 * terrain's costs.
 *
 * Coordinates are ordered x, elevation, y for the movement subsystem. */
void battle_script_walk_to_event_instruction(const event_walk_to_parameters_t* parameters) {
    s32 id;
    s16 coordinates[3];

    id = battle_script_load_halfword(parameters->unit_id_le);
    id = battle_script_get_misc_id(id);
    if (id != EVENT_MISC_ID_NONE) {
        coordinates[0] = parameters->x;
        coordinates[2] = parameters->y;
        coordinates[1] = parameters->elevation;
        battle_move_start_unit_walk_to(id, coordinates, 0x100 - (parameters->use_terrain_costs << 8), 3,
            battle_script_load_halfword(parameters->speed_le));
    }
}
