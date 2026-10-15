#include "fft/battle.h"
#include "psx/types.h"

/* Walk on from the tile edge to the centre after a jump has landed (battle_move_update_landing_phase):
 * start the step with battle_move_start_unit_step, or at climb speed when it carries
 * BATTLE_MOVE_STEP_DESTINATION_CLIMB, and go straight to its edge-to-centre phase for the unit's
 * direction. */
void battle_move_resume_walk_to_center(battle_unit_misc_data_t* unit) {
    s32 direction;

    direction = battle_move_get_direction(unit);
    switch (g_battle_move_step_value.bits.climb) {
    case 0:
        battle_move_start_unit_step(unit, g_battle_move_current_tile, g_battle_move_destination_tile);
        unit->step_phase = g_battle_move_walk_to_centre_phases[direction];
        break;
    case 1:
        battle_move_start_unit_step_at_climb_speed(unit, g_battle_move_current_tile, g_battle_move_destination_tile);
        unit->step_phase = g_battle_move_climb_to_centre_phases[direction];
        break;
    }
}
