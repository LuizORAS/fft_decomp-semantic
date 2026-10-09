#include "fft/battle.h"

/* Start event opcode 0x28 movement for one unit.
 *
 * Coordinates are ordered x, elevation, y for the movement subsystem. The
 * final payload bit selects absolute elevation or a one-level addend. */
void battle_script_walk_to_thread(const event_walk_to_parameters_t* parameters) {
    s32 id;
    s16 coordinates[3];

    id = battle_script_load_halfword(parameters->unit_id_le);
    id = battle_get_misc_id(id);
    if (id != EVENT_MISC_ID_NONE) {
        coordinates[0] = parameters->x;
        coordinates[2] = parameters->y;
        coordinates[1] = parameters->elevation;
        battle_move_start_unit_walk_to(id, coordinates, 0x100 - (parameters->elevation_flag << 8), 3,
            battle_script_load_halfword(parameters->speed_le));
    }
}
