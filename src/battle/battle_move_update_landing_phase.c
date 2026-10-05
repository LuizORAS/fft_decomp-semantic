#include "fft/battle.h"
#include "psx/types.h"

/* Jump landing phases (0x14, 0x18, 0x1c, 0x20; set by battle_move_finish_unit_step_at_tile_edge):
 * wait while landing animation 0x20 plays, then walk on to the tile centre
 * (battle_move_resume_walk_to_center). */
void battle_move_update_landing_phase(battle_unit_misc_data_t* unit) {
    if (((unit->encoded_animation >> 1) != 0x20) || (unit->animation_countdown == 0)) {
        battle_move_resume_walk_to_center(unit);
    }
}
