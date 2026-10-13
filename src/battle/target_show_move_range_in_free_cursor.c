#include "fft/battle.h"
#include "fft/battle_camera.h"
#include "fft/battle_move.h"
#include "fft/battle_target.h"
#include "psx/types.h"

/* Enter the display-move-area state for the unit picked with the free cursor:
 * compute and show its reachable tiles, and switch the cursor input to
 * free-cursor mode with the previous mode saved. */
void target_show_move_range_in_free_cursor(void) {
    battle_unit_t* unit;
    battle_stats_t* stats;
    s32 prev;

    camera_enable_map_pan();
    g_battle_game_state = BATTLE_GAME_STATE_DISPLAY_MOVE_AREA;
    unit = battle_unit_get_casting();
    stats = unit->battle_data;
    move_set_reachable_tiles(stats->battle_id, unit->map_x, unit->map_y, unit->map_z);
    target_set_tile_background_color(BATTLE_TILE_MARK_SHOW_MOVE, 1);
    prev = g_battle_cursor_input_mode;
    g_battle_cursor_input_mode = BATTLE_CURSOR_INPUT_FREE_CURSOR;
    g_battle_cursor_input_mode_saved = prev;
    main_sound_play_sfx(MAIN_SFX_CONFIRM);
}
