#include "fft/battle.h"
#include "fft/battle_camera.h"
#include "fft/battle_script.h"

/* CONTINUE_TURN: once the camera has stopped (no step vector, no rotation), decide what follows the
 * command; EXP and JP were already granted in RESUME_ATTACK_PHASE. After a charged ability (0x200) a
 * scenario event with a nonzero finish operation may start, or else the between-turn events run. When
 * the unit's turn is over (battle_turn_is_over returns 1), its command is restored from its battle
 * record; a Jump then resets its preparation and goes to the between-turn events, while other commands
 * go on to Wait (battle_state_enter_wait). Otherwise its action menus reopen. */
void battle_state_handle_continue_turn_state(void) {
    battle_unit_t* unit;

    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    unit = battle_unit_get_source();
    if ((g_battle_camera_step_vector.vx | g_battle_camera_step_vector.vy | g_battle_camera_step_vector.vz) != 0) {
        return;
    }
    if (g_battle_camera_rotation_action != BATTLE_CAMERA_ROTATION_NONE) {
        return;
    }
    if (g_battle_turn_event == BATTLE_TURN_EVENT_ABILITY_READY) {
        if (battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_SCENARIO_CHECK, 2, unit->stats->battle_id, 0, 1)
                == 2
            && battle_script_get_event_finish_operation() != 0) {
            g_battle_previous_game_state = g_battle_game_state;
            battle_menu_init_system_function(
                BATTLE_MENU_SYSTEM_COMMAND_SCENARIO_CHECK, 0, unit->stats->battle_id, 0, 1);
            battle_state_enter_event();
            return;
        }
        battle_turn_advance();
        return;
    }
    if (battle_turn_is_over(unit->stats->battle_id) == 1) {
        /* battle_stats_t +0x16e holds the same 20-byte command payload. */
        unit->command_state.ai.data.action = unit->stats->action_command;
        unit->used_ability_id = unit->stats->action_command.ability_id;
        if (unit->attack_phase_state == BATTLE_ATTACK_PREPARATION_JUMP) {
            unit->attack_phase_state = BATTLE_ATTACK_PREPARATION_CHARGE;
            battle_gfx_reset_unit_graphic_trigger(unit->runtime_id);
            battle_turn_advance();
            return;
        }
        battle_state_enter_wait();
        return;
    }
    battle_state_enter_open_action_menus();
}
