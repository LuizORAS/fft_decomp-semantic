#include "fft/battle.h"
#include "fft/world.h"

/* Start event opcode 0x28 movement for one unit.
 *
 * Only the block thread calls this, through the main-stack callback bridge:
 * world_script_execute_event recognizes the opcode and does nothing. */
void world_script_walkto_event_instruction(const event_walk_to_parameters_t* parameters) {
    unit_tile_coordinates_t coords;
    s32 misc_id;

    misc_id = world_script_load_halfword(parameters->unit_id_le);
    misc_id = world_script_get_misc_id(misc_id);
    if (misc_id != EVENT_MISC_ID_NONE) {
        coords.x = parameters->x;
        coords.y = parameters->y;
        coords.elevation = parameters->elevation;
        battle_move_start_unit_walk_to(misc_id, &coords, 0x100 - (parameters->elevation_flag << 8), 3,
            world_script_load_halfword(parameters->speed_le));
    }
}
