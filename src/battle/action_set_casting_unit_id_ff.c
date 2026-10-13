#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_menu.h"
#include "fft/battle_script.h"

/* Hand the battle over to an event script: enter the event state at animation
 * speed 2, reset the jump graphics and the event state, disable the status menu
 * and clear the casting and source unit ids (0xff). */
void action_set_casting_unit_id_ff(void) {
    g_main_system_animation_speed = 2;
    g_battle_game_state = BATTLE_GAME_STATE_EVENT;
    battle_gfx_reset_jumping_unit_graphic_triggers();
    script_reset_event_state();
    action_clear_at_list_active();
    g_battle_menu_status_enabled = 0;
    g_battle_casting_runtime_id = 0xff;
    g_battle_unit_source_runtime_id = 0xff;
}
