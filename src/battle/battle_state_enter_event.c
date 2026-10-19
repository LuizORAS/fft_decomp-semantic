#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_menu.h"
#include "fft/battle_script.h"

/* Enter EVENT at 30 fps for an event script: reset the jumping units' graphic triggers and the
 * event state, hide the map cursor, turn the status display off (g_battle_menu_status_enabled)
 * and clear the casting and source unit ids (0xff). */
void battle_state_enter_event(void) {
    g_main_system_vsync_interval = 2;
    g_battle_game_state = BATTLE_GAME_STATE_EVENT;
    battle_gfx_reset_jumping_unit_graphic_triggers();
    script_reset_event_state();
    battle_target_hide_cursor();
    g_battle_menu_status_enabled = 0;
    g_battle_casting_runtime_id = 0xff;
    g_battle_unit_source_runtime_id = 0xff;
}
