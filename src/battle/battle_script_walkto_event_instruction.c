#include "fft/battle.h"

/* Start event opcode 0x28 movement. The final operand enables terrain costs;
 * the separate destination-layer byte selects elevation. */
void battle_script_walkto_event_instruction(const event_walk_to_parameters_t* parameters) {
    s32 id;
    unit_tile_coordinates_t coordinates;

    id = battle_script_load_halfword(parameters->unit_id_le);
    id = battle_script_get_misc_id(id);
    if (id != EVENT_MISC_ID_NONE) {
        coordinates.x = parameters->x;
        coordinates.y = parameters->y;
        coordinates.elevation = parameters->elevation;
        battle_move_start_unit_walk_to(id, &coordinates,
            BATTLE_MOVE_PATHFIND_IGNORE_TERRAIN_COST
                - parameters->terrain_cost_flag * BATTLE_MOVE_PATHFIND_IGNORE_TERRAIN_COST,
            3, battle_script_load_halfword(parameters->speed_le));
    }
}
