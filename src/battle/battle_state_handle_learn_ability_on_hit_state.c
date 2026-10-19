#include "fft/battle.h"

/* LEARN_ABILITY_ON_HIT: the post-action reports, one per answer: level up (phase 0), job level
 * up (1) and a unit learning the ability on hit (2, learned on 7). After phase 2, once no
 * effect runs and the number displays end, set the source unit's animation and finish the
 * command. */
void battle_state_handle_learn_ability_on_hit_state(void) {
    battle_ai_command_t* command_address;
    s32 command;
    s32 counter;
    s32 skip_animation;
    s32 hit_count;
    battle_unit_t* casting_unit;
    battle_unit_t* target_unit;
    u8 compiler_stack_pad[8];

    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    command_address = battle_menu_get_selected_command_address();
    casting_unit = battle_unit_get_casting();
    command = command_address->kind;
    switch (command) {
    case 7:
        if (g_battle_action_post_action_display_phase == BATTLE_POST_ACTION_DISPLAY_LEARN_ON_HIT) {
            target_unit = battle_unit_get_by_battle_id(g_battle_action_learn_on_hit_unit_id);
            battle_unit_learn_used_ability(target_unit->stats);
        }
    case 8:
    case 0xff:
        g_battle_action_post_action_display_phase += 1;
        if (g_battle_action_post_action_display_phase >= 3) {
            g_battle_action_post_action = 1;
        } else {
            battle_action_init_learn_ability_on_hit();
        }
        break;
    }
    if (g_battle_action_post_action != 0 && g_battle_state_animation_continue_check == 0) {
        counter = 0;
        if (casting_unit->numeric_display.active == 0) {
            hit_count = casting_unit->strike.target_count;
            skip_animation = 0;
            if (hit_count > 0) {
                do {
                    target_unit = battle_unit_get_by_battle_id(casting_unit->strike.target_list[counter]);
                    if (target_unit != 0 && target_unit->numeric_display.active != 0) {
                        skip_animation = 1;
                        break;
                    }
                    hit_count = casting_unit->strike.target_count;
                    counter += 1;
                } while (counter < hit_count);
            }
            if (skip_animation == 0) {
                casting_unit = battle_unit_get_source();
                battle_unit_call_set_animation_based_on_status(casting_unit);
                battle_unit_update_post_command_animation_display(casting_unit);
            }
        }
    }
}
