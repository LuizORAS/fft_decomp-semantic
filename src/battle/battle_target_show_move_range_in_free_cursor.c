#include "fft/battle.h"
#include "fft/battle_camera.h"
#include "fft/battle_move.h"
#include "fft/battle_target.h"
#include "psx/types.h"

/* From the free cursor, show the move range of the unit picked there (DISPLAY_MOVE_AREA): its
 * reachable tiles (battle_move_set_reachable_tiles), tinted as the move range. */
void battle_target_show_move_range_in_free_cursor(void) {
    battle_unit_t* unit;
    battle_stats_t* stats;
    s32 prev;

    battle_camera_enable_map_pan();
    g_battle_game_state = BATTLE_GAME_STATE_DISPLAY_MOVE_AREA;
    unit = battle_unit_get_casting();
    stats = unit->stats;
    battle_move_set_reachable_tiles(stats->battle_id, unit->map_x, unit->map_y, unit->map_z);
    battle_target_tint_marked_tiles(BATTLE_TILE_MARK_SHOW_MOVE, 1);
    prev = g_battle_cursor_input_mode;
    g_battle_cursor_input_mode = BATTLE_CURSOR_INPUT_FREE_CURSOR;
    g_battle_cursor_input_mode_saved = prev;
    main_sound_play_sfx(MAIN_SFX_CONFIRM);
}
