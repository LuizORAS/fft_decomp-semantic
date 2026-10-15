#include "fft/battle.h"
#include "psx/types.h"

/* From the free cursor, show the move range of the unit picked there (DISPLAY_MOVE_AREA): its
 * reachable tiles (battle_move_set_reachable_tiles), tinted as the move range. */
void battle_target_show_move_range_in_free_cursor(void) {
    battle_unit_misc_data_t* misc;
    battle_stats_t* stats;
    s32 prev;

    battle_state_enable_camera_pan();
    g_battle_game_state = BATTLE_GAME_STATE_DISPLAY_MOVE_AREA;
    misc = battle_unit_get_casting_misc_data();
    stats = misc->battle_data;
    battle_move_set_reachable_tiles(stats->misc_unit_id, misc->map_x, misc->map_y, misc->map_z);
    battle_target_tint_marked_tiles(BATTLE_TILE_MARK_SHOW_MOVE, 1);
    prev = g_battle_cursor_input_mode;
    g_battle_cursor_input_mode = BATTLE_CURSOR_INPUT_FREE_CURSOR;
    g_battle_cursor_input_mode_saved = prev;
    main_sound_play_sfx(MAIN_SFX_CONFIRM);
}
