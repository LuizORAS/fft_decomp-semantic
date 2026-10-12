#include "fft/battle_move.h"
#include "fft/battle_script.h"

/* Start event opcode 0x28 movement. The final operand enables terrain costs;
 * the separate destination-layer byte selects elevation. */
void script_walkto_event_instruction(const event_walk_to_parameters_t* parameters) {
    s32 id;
    map_tile_coordinates_t coordinates;

    id = script_load_halfword(parameters->unit_id_le);
    id = script_get_misc_id(id);
    if (id != EVENT_MISC_ID_NONE) {
        coordinates.x = parameters->x;
        coordinates.y = parameters->y;
        coordinates.layer = parameters->elevation;
        move_start_unit_walk_to(id, &coordinates,
            MOVE_PATHFIND_IGNORE_TERRAIN_COST - parameters->terrain_cost_flag * MOVE_PATHFIND_IGNORE_TERRAIN_COST, 3,
            script_load_halfword(parameters->speed_le));
    }
}
