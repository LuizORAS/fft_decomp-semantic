#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_camera.h"
#include "fft/battle_target.h"

/* Enter FREE_CURSOR at 60 fps with the camera following the cursor: store the cursor unit's
 * name and data, show the map cursor and store the source unit's ENTD flags without the
 * control flag. */
void battle_state_enter_free_cursor(void) {
    battle_unit_t* unit;

    g_main_system_vsync_interval = 1;
    battle_camera_disable_map_pan();
    g_battle_game_state = BATTLE_GAME_STATE_FREE_CURSOR;
    g_battle_menu_help_opening = 0;
    battle_target_show_cursor_unit_panel();
    battle_target_show_cursor();
    unit = battle_unit_get_source();
    if (unit != 0) {
        battle_unit_store_entd_flags_without_control_flag(unit);
    }
}
