#include "fft/battle.h"

/* Run the acting unit's confirmed command: switch the unit panels to mode 0, mark the unit as having
 * acted (which can end its turn), face it toward the cursor and commit the command
 * (battle_action_commit_command). The result goes to attack_phase_state: 0 (charging) shows the
 * charge pose and ends the command (AFTER_COMMAND, through
 * battle_unit_update_post_command_animation_display); 1 (acts now) or 3 (Jump) enters
 * COMMENCE_ATTACK_PHASE, with the charge pose when the ability has a charge animation; 2 (Change
 * Equipment) or another value reports pointer exception 0x13 and ends the command. */
void battle_action_execute_ability(void) {
    battle_unit_misc_data_t* unit;
    s32 result;

    battle_menu_store_unit_names_and_event_block_data(0, 0, 0);
    unit = battle_unit_get_source_misc_data();
    battle_action_set_only_action_taken(unit->battle_data->misc_unit_id);
    unit->ability_ct_resolved |= 2;
    battle_unit_face_toward_cursor(unit);
    result = battle_action_commit_command((u8*)&unit->command_state.ai.data);
    unit->attack_phase_state = result;
    switch (result) {
    case 0:
        battle_unit_start_ability_charge_animation_for_movement(unit);
        break;
    case 1:
    case 3:
        battle_state_disable_camera_pan();
        g_battle_game_state = BATTLE_GAME_STATE_COMMENCE_ATTACK_PHASE;
        g_battle_action_post_action = 0;
        unit->ability_ct_resolved |= 2;
        if (g_battle_ability_animation_data[unit->used_ability_id].charge_animation_set_id != 0) {
            battle_unit_start_ability_charge_animation_for_movement(unit);
        }
        return;
    case 2:
    default:
        main_system_handle_pointer_exception(0x13);
        break;
    }
    battle_unit_update_post_command_animation_display(unit);
}
