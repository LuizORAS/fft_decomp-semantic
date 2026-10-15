#include "fft/battle.h"

/* ACTION_EXECUTE state: animate the units and camera (not while the status screen is up),
 * advance the caster's state counter by the VSync interval and each target's knockback step. The
 * strike has settled when no effect or target step runs
 * (g_battle_state_animation_continue_check), the caster shows no number and its animation
 * countdown is 0 or its animation id is below 0x3c; then the targets' displays are refreshed and
 * the battle message starts. An ability other than Attack (id 0), Knockback and Throw Stone
 * moves the camera to the cursor tile only when it has no effect file or is a Charge, and
 * restarts the background sound on settling; a reaction ability with no effect file and its own
 * attack animation starts each target's post-attack display instead of the refresh. Attack,
 * Knockback and Throw Stone always follow the cursor tile and run the menu layer each frame;
 * Knockback prepares each target's post-action display first. */
void battle_state_handle_action_execute_state(void) {
    battle_unit_misc_data_t* unit;
    battle_unit_misc_data_t* target;
    s32 i;

    battle_gfx_update_all_unit_rotation_and_vectors();
    if (g_battle_menu_status_screen_selected != 1) {
        battle_unit_update_and_animate_units();
        battle_gfx_update_screen_color_modulation_fade();
        battle_camera_update_real_coord_animation();
        battle_camera_update_offset_screen_coord_animation();
        battle_camera_update_zoom_animation();
        battle_camera_update_rotation_animation();
    }
    battle_gfx_update_status_bubbles_and_graphics();
    battle_gfx_draw_screen_color_modulation_overlay();
    battle_target_update_cursor();

    unit = battle_unit_get_casting_misc_data();
    /* The target reads the halfword of g_main_system_vsync_interval. */
    unit->state_frame_counter += g_main_system_vsync_interval;
    if (unit->target_count != 0) {
        for (i = 0; i < unit->target_count; i++) {
            target = battle_unit_get_misc_data_by_battle_id(unit->target_list[i]);
            if (target != 0) {
                battle_move_update_knockback_step(target);
                g_battle_state_animation_continue_check |= target->step_phase;
            }
        }
    }

    if (unit->used_ability_id != 0 && unit->used_ability_id != ABILITY_ID_KNOCKBACK
        && unit->used_ability_id != ABILITY_ID_BASIC_SKILL_THROW_STONE) {
        if (battle_effect_ability_has_no_effect(unit->used_ability_id) != 0
            || (unit->used_ability_id >= ABILITY_ID_CHARGE_FIRST && unit->used_ability_id < ABILITY_ID_MATH_FIRST)) {
            battle_camera_update_cursor_tile_vector();
        }
        if (g_battle_state_animation_continue_check == 0 && unit->numeric_display_active == 0
            && (unit->animation_wait == 0 || (unit->sequence_id >> 1) < 0x3cU)) {
            main_sound_replay_bg_sfx();
            if (unit->target_count != 0) {
                for (i = 0; i < unit->target_count; i++) {
                    target = battle_unit_get_misc_data_by_battle_id(unit->target_list[i]);
                    if (target != 0) {
                        if (unit->used_ability_id >= ABILITY_ID_REACTION_FIRST
                            && unit->used_ability_id < ABILITY_ID_SUPPORT_FIRST) {
                            if (battle_effect_ability_has_no_effect(unit->used_ability_id) != 0
                                && g_battle_ability_animation_data[unit->used_ability_id].attack_animation
                                    != BATTLE_ATTACK_ANIMATION_USE_WEAPON) {
                                /* Both arguments are misc records overlaying the declared parameters. */
                                battle_unit_start_post_attack_animation_display((s32)unit, (u8*)target);
                            }
                        } else {
                            battle_unit_update_display_by_misc_id(target->unit_id);
                        }
                    }
                }
            }
            battle_state_enter_battle_message_display();
        }
    } else {
        battle_camera_update_cursor_tile_vector();
        battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
        if (g_battle_state_animation_continue_check == 0 && unit->numeric_display_active == 0
            && (unit->animation_wait == 0 || (unit->sequence_id >> 1) < 0x3cU)) {
            if (unit->target_count != 0) {
                for (i = 0; i < unit->target_count; i++) {
                    target = battle_unit_get_misc_data_by_battle_id(unit->target_list[i]);
                    if (target != 0) {
                        if (unit->used_ability_id == ABILITY_ID_KNOCKBACK) {
                            battle_gfx_prepare_post_action_display_by_misc_id(target->unit_id);
                        }
                        battle_unit_update_display_by_misc_id(target->unit_id);
                    }
                }
            }
            battle_state_enter_battle_message_display();
        }
    }
}
