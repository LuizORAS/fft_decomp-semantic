#include "fft/battle.h"
#include "fft/battle_camera.h"
#include "fft/battle_target.h"

/* Start the source unit's AI turn (AI_COMMAND): reset its state counter and command readiness,
 * clear the tile colors and store its names for the menus. */
void battle_state_enter_ai_command(void) {
    battle_unit_t* unit;

    battle_camera_disable_map_pan();
    g_battle_game_state = BATTLE_GAME_STATE_AI_COMMAND;
    unit = battle_unit_get_source();
    unit->state_frame_counter = 0;
    unit->command_ready = 0;
    battle_target_tint_marked_tiles(BATTLE_TILE_MARK_NONE, 0);
    if (unit != 0 && unit->stats != 0) {
        battle_unit_start_view_thread(UNIT_VIEW_MODE_UNIT_SUMMARY, unit->stats->battle_id, 0);
    }
}
