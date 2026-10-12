#include "fft/battle_move.h"
#include "fft/world.h"

/* Start event opcode 0x28 movement for one unit.
 *
 * Only the block thread calls this, through the main-stack callback bridge:
 * world_script_execute_event recognizes the opcode and does nothing. */
void world_script_walkto_event_instruction(const event_walk_to_parameters_t* parameters) {
    map_tile_coordinates_t coords;
    s32 misc_id;

    misc_id = world_script_load_halfword(parameters->unit_id_le);
    misc_id = world_script_get_misc_id(misc_id);
    if (misc_id != EVENT_MISC_ID_NONE) {
        coords.x = parameters->x;
        coords.y = parameters->y;
        coords.layer = parameters->elevation;
        move_start_unit_walk_to(misc_id, &coords,
            MOVE_PATHFIND_IGNORE_TERRAIN_COST - parameters->terrain_cost_flag * MOVE_PATHFIND_IGNORE_TERRAIN_COST, 3,
            world_script_load_halfword(parameters->speed_le));
    }
}
