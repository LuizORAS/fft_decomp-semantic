#include "fft/battle.h"

/* Rare/common item result copied from battle_map_determine_rare_common_item. */
typedef struct battle_move_found_item {
    s32 _unused_00;
    u8 item_id; /* 0x4 */
} battle_move_found_item_t;

extern battle_move_found_item_t g_battle_move_find_display_item;

/* Start the next pending post-move event (g_battle_move_post_move_events) and enter
 * BATTLE_GAME_STATE_CRYSTAL_LEARN, whose handler clears the event's bit once it has played and calls
 * this again. Order: a crystal or chest pickup, the movement-ability rewards and level-up reports
 * (two display phases), a found item, a trap, a cancelled charge, then a display refresh of the
 * mover (SOURCE_DISPLAY, which nothing sets) or of its mount (MOUNT_STATUS_CHANGE). Returns 1 when
 * an event started; with none left it refreshes both units, enters the after-command state unless
 * the Deep Dungeon refresh takes over, and returns 0. */
s32 battle_move_start_next_post_move_event(void) {
    battle_unit_misc_data_t* source;
    battle_unit_misc_data_t* casting;
    battle_unit_misc_data_t* target;
    battle_stats_t* stats;
    s32 item;
    s32 animation;
    s32 trap;

    source = battle_unit_get_source_misc_data();
    casting = battle_unit_get_casting_misc_data();
    battle_menu_init_system_function(0xc, 0, source->battle_data->misc_unit_id, 0, 0);
    g_battle_game_state = BATTLE_GAME_STATE_CRYSTAL_LEARN;
    g_battle_action_post_action = 0;
    casting->state_frame_counter = 0;
    if (g_battle_move_post_move_events & BATTLE_MOVE_POST_EVENT_CRYSTAL_OR_TREASURE) {
        target = battle_unit_get_crystal_or_treasure_at_map_coords(source->map_x, source->map_y, source->map_z);
        item = battle_menu_get_dead_unit_selection(source->battle_data->misc_unit_id);
        if (item == -1) {
            animation = 0x1b;
        } else {
            battle_gfx_store_item_display_data((battle_gfx_render_unit_t*)source, item);
            animation = 0x39;
        }
        battle_unit_store_animation_facing_movement_data(animation, source->facing, source);
        if (item == -1) {
            stats = source->battle_data;
            /* The target reads 0x1ba as a halfword for this test; the u8
             * field access emits lbu. */
            if ((*(u16*)&stats->initial_team_flags & 0x38) == 8) {
                battle_menu_init_system_function(0xc, stats->misc_unit_id, stats->misc_unit_id, 0, 1);
            } else {
                battle_menu_init_system_function(0xc, stats->misc_unit_id, stats->misc_unit_id, 0, 0);
                battle_unit_learn_from_crystal(source->battle_data, 2);
                battle_action_finalize_attack_and_flag_reactions(source->battle_data->misc_unit_id);
                battle_gfx_prepare_post_action_display(source);
                battle_unit_update_display_by_misc_id(source->unit_id);
            }
        } else {
            if (!(source->battle_data->initial_team_flags & BATTLE_TEAM_MASK)) {
                battle_menu_init_system_function(
                    0xc, source->battle_data->misc_unit_id, source->battle_data->misc_unit_id, 0, 1);
            } else {
                battle_menu_init_system_function(
                    0xc, source->battle_data->misc_unit_id, source->battle_data->misc_unit_id, 0, 0);
            }
            if ((*(u16*)&source->battle_data->initial_team_flags & 0x38) != 8) {
                battle_unit_learn_from_crystal(source->battle_data, 4);
            }
        }
        battle_gfx_configure_misc_unit_palette_modulation_1f(target->unit_id);
        target->depth_height_offset
            = 0x24 - (battle_gfx_calculate_screen_z_from_misc_screen_data(source) - target->screen.vy);
        main_sound_play_sfx_find_channel(0x85);
        return 1;
    }
    if (g_battle_move_post_move_events & BATTLE_MOVE_POST_EVENT_MOVEMENT_BENEFIT) {
        switch (g_battle_action_post_action_display_phase) {
        case 0:
            battle_action_init_movement_ability_benefit(casting->battle_data);
            battle_unit_build_gained_exp_jp_level_job_level(casting->battle_data, &casting->action_rewards);
            battle_gfx_prepare_post_action_display(casting);
            battle_gfx_init_earned_exp_jp_display(casting);
            battle_action_report_level_up(casting);
            return 1;
        case 1:
            battle_action_report_job_level_up(casting);
            return 1;
        }
        return 1;
    }
    if (g_battle_move_post_move_events & BATTLE_MOVE_POST_EVENT_ITEM_FOUND) {
        g_battle_move_find_display_item
            = *(battle_move_found_item_t*)battle_map_determine_rare_common_item(casting->battle_data);
        battle_gfx_store_item_display_data((battle_gfx_render_unit_t*)source, g_battle_move_find_display_item.item_id);
        battle_unit_store_animation_facing_movement_data(0x33, source->facing, source);
        if (!(source->team_flags & BATTLE_TEAM_MASK)) {
            battle_menu_init_system_function(
                0xf, g_battle_move_find_display_item.item_id, source->battle_data->misc_unit_id, 0, 1);
            main_sound_play_tune(2);
            return 1;
        }
        battle_menu_init_system_function(
            0xf, g_battle_move_find_display_item.item_id, source->battle_data->misc_unit_id, 0, 0);
        return 1;
    }
    if (g_battle_move_post_move_events & BATTLE_MOVE_POST_EVENT_TRAP_TRIGGERED) {
        trap = battle_action_apply_tile_trap(casting->battle_data);
        if ((source->pending_attack_result
                = battle_action_finalize_attack_and_flag_reactions(source->battle_data->misc_unit_id))
            == -1) {
            battle_unit_find_relocation_tile(source->battle_data->misc_unit_id, &source->dismount);
            battle_unit_set_map_coords_after_death_dismount(source);
        }
        battle_gfx_prepare_post_action_display(source);
        battle_unit_set_target_animation_from_attack_type(0, source);
        battle_effect_set_secondary_venom_trap(source);
        battle_menu_init_system_function(0xe, trap, source->battle_data->misc_unit_id, 0, 1);
        battle_ai_record_considered_coords(casting->map_x, casting->map_z, casting->map_y);
        return 1;
    }
    if (g_battle_move_post_move_events & BATTLE_MOVE_POST_EVENT_CHARGING_CANCEL) {
        battle_status_remove_charging_ability_ct(casting->battle_data, 1);
        battle_menu_init_system_function(0xc, 0, source->battle_data->misc_unit_id, 0, 0);
        return 1;
    }
    if (g_battle_move_post_move_events & BATTLE_MOVE_POST_EVENT_SOURCE_DISPLAY) {
        battle_menu_init_system_function(0xc, 0, source->battle_data->misc_unit_id, 0, 0);
        battle_gfx_prepare_post_action_display_by_misc_id(source->unit_id);
        battle_unit_update_display_by_misc_id(source->unit_id);
        return 1;
    }
    if (g_battle_move_post_move_events & BATTLE_MOVE_POST_EVENT_MOUNT_STATUS_CHANGE) {
        battle_menu_init_system_function(0xc, 0, source->battle_data->misc_unit_id, 0, 0);
        if (source->mount_state != 0) {
            target = battle_unit_get_misc_data_by_misc_id(source->mount_partner_misc_id);
            if (target != 0) {
                battle_gfx_prepare_post_action_display_by_misc_id(target->unit_id);
                battle_unit_update_display_by_misc_id(target->unit_id);
            }
        }
        return 1;
    }
    battle_unit_update_display_by_misc_id(source->unit_id);
    battle_unit_update_display_by_misc_id(casting->unit_id);
    if (battle_map_refresh_deep_dungeon() == 0) {
        battle_state_enter_after_command();
    }
    return 0;
}
