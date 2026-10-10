#include "fft/battle.h"

void battle_state_handle_commence_attack_phase_state(void) {
    battle_unit_misc_data_t* unit;

    battle_state_handle_free_cursor_input();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    unit = battle_unit_get_source_misc_data();
    if (unit->attack_phase_state == BATTLE_ATTACK_PREPARATION_JUMP) {
        if (unit->animation_wait == 0) {
            battle_action_set_only_action_taken(unit->battle_data->misc_unit_id);
            battle_unit_update_post_command_animation_display(unit);
        }
    } else {
        g_battle_action_phase = BATTLE_ACTION_PHASE_PRE_ACTION;
        battle_state_announce_next_charged_action();
    }
}
