#include "fft/battle.h"
#include "fft/battle_map.h"
#include "fft/battle_menu.h"
#include "fft/battle_move.h"

/* Starts the next post-action step selected by g_move_find_result.
 *
 * Handles, in priority order, the crystal/treasure pickup (bit 0x1), the
 * EXP/JP and level-up reports (0x2), a found rare/common item (0x4), a tile
 * trap (0x8), a cancelled charge (0x10) and the post-action displays for the
 * source (0x200) or its mount partner (0x400). Returns 1 when a step was
 * started, otherwise refreshes both units, re-enters the after-command state
 * unless the Deep Dungeon map refresh takes over, and returns 0. */
s32 move_start_next_post_movement_step(void) {
    battle_unit_misc_data_t* source;
    battle_unit_misc_data_t* casting;
    battle_unit_misc_data_t* target;
    battle_stats_t* stats;
    s32 item;
    s32 animation;
    s32 trap;

    source = battle_unit_get_source_misc_data();
    casting = battle_unit_get_casting_misc_data();
    battle_menu_init_system_function(
        BATTLE_MENU_SYSTEM_COMMAND_CRYSTAL_OR_TREASURE, 0, source->battle_data->misc_unit_id, 0, 0);
    g_battle_game_state = BATTLE_GAME_STATE_CRYSTAL_LEARN;
    g_action_post_action = 0;
    casting->state_frame_counter = 0;
    if (g_move_find_result & MOVE_POST_EVENT_CRYSTAL_OR_TREASURE) {
        target = battle_unit_get_crystal_or_treasure_at_map_coords(source->map_x, source->map_y, source->map_z);
        item = battle_menu_get_dead_unit_selection(source->battle_data->misc_unit_id);
        if (item == -1) {
            animation = BATTLE_UNIT_ANIMATION_RAISE_HANDS;
        } else {
            battle_gfx_store_item_display_data(source, item);
            animation = BATTLE_UNIT_ANIMATION_HOLD_ITEM;
        }
        battle_unit_store_animation_facing_movement_data(animation, source->facing, source);
        if (item == -1) {
            stats = source->battle_data;
            if (!(stats->initial_team_flags & BATTLE_TEAM_MASK)
                && (stats->initial_team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED)) {
                battle_menu_init_system_function(
                    BATTLE_MENU_SYSTEM_COMMAND_CRYSTAL_OR_TREASURE, stats->misc_unit_id, stats->misc_unit_id, 0, 1);
            } else {
                battle_menu_init_system_function(
                    BATTLE_MENU_SYSTEM_COMMAND_CRYSTAL_OR_TREASURE, stats->misc_unit_id, stats->misc_unit_id, 0, 0);
                battle_unit_learn_from_crystal(source->battle_data, BATTLE_CRYSTAL_RESTORE_HP_MP);
                action_finalize_attack_and_flag_reactions(source->battle_data->misc_unit_id);
                battle_gfx_prepare_post_action_display(source);
                battle_unit_update_display_by_misc_id(source->unit_id);
            }
        } else {
            if (!(source->battle_data->initial_team_flags & BATTLE_TEAM_MASK)) {
                battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_CRYSTAL_OR_TREASURE,
                    source->battle_data->misc_unit_id, source->battle_data->misc_unit_id, 0, 1);
            } else {
                battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_CRYSTAL_OR_TREASURE,
                    source->battle_data->misc_unit_id, source->battle_data->misc_unit_id, 0, 0);
            }
            if ((source->battle_data->initial_team_flags & BATTLE_TEAM_MASK)
                || !(source->battle_data->initial_team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED)) {
                battle_unit_learn_from_crystal(source->battle_data, BATTLE_CRYSTAL_TAKE_TREASURE);
            }
        }
        battle_gfx_configure_misc_unit_palette_modulation_1f(target->unit_id);
        target->depth_height_offset
            = 0x24 - (battle_gfx_calculate_screen_z_from_misc_screen_data(source) - target->screen.vy);
        main_sound_play_sfx_find_channel(MAIN_SFX_COMPLETE);
        return 1;
    }
    if (g_move_find_result & MOVE_POST_EVENT_MOVEMENT_BENEFIT) {
        switch (g_action_post_action_display_phase) {
        case BATTLE_POST_ACTION_DISPLAY_LEVEL_UP:
            action_init_movement_ability_benefit(casting->battle_data);
            battle_unit_build_gained_exp_jp_level_job_level(casting->battle_data, &casting->strike.action_rewards);
            battle_gfx_prepare_post_action_display(casting);
            battle_gfx_init_earned_exp_jp_display(casting);
            action_report_level_up(casting);
            return 1;
        case BATTLE_POST_ACTION_DISPLAY_JOB_LEVEL_UP:
            action_report_job_level_up(casting);
            return 1;
        }
        return 1;
    }
    if (g_move_find_result & MOVE_POST_EVENT_ITEM_FOUND) {
        g_move_find_display_item = *map_determine_rare_common_item(casting->battle_data);
        battle_gfx_store_item_display_data(source, g_move_find_display_item.item_id);
        battle_unit_store_animation_facing_movement_data(BATTLE_UNIT_ANIMATION_FOUND_ITEM, source->facing, source);
        if (!(source->team_flags & BATTLE_TEAM_MASK)) {
            battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_MOVE_FIND_ITEM,
                g_move_find_display_item.item_id, source->battle_data->misc_unit_id, 0, 1);
            main_sound_play_tune(MAIN_SOUND_TUNE_ITEM_FOUND);
            return 1;
        }
        battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_MOVE_FIND_ITEM, g_move_find_display_item.item_id,
            source->battle_data->misc_unit_id, 0, 0);
        return 1;
    }
    if (g_move_find_result & MOVE_POST_EVENT_TRAP_TRIGGERED) {
        trap = action_apply_tile_trap(casting->battle_data);
        if ((source->pending_attack_result
                = action_finalize_attack_and_flag_reactions(source->battle_data->misc_unit_id))
            == -1) {
            battle_unit_find_relocation_tile(source->battle_data->misc_unit_id, &source->dismount);
            battle_unit_set_map_coords_after_death_dismount(source);
        }
        battle_gfx_prepare_post_action_display(source);
        battle_unit_set_target_animation_from_attack_type(0, source);
        effect_set_secondary_venom_trap(source);
        battle_menu_init_system_function(
            BATTLE_MENU_SYSTEM_COMMAND_TRAP_RESULT, trap, source->battle_data->misc_unit_id, 0, 1);
        ai_record_triggered_trap_coords(casting->map_x, casting->map_z, casting->map_y);
        return 1;
    }
    if (g_move_find_result & MOVE_POST_EVENT_CHARGING_CANCEL) {
        status_remove_charging_ability_ct(casting->battle_data, 1);
        battle_menu_init_system_function(
            BATTLE_MENU_SYSTEM_COMMAND_CRYSTAL_OR_TREASURE, 0, source->battle_data->misc_unit_id, 0, 0);
        return 1;
    }
    if (g_move_find_result & MOVE_POST_EVENT_SOURCE_DISPLAY) {
        battle_menu_init_system_function(
            BATTLE_MENU_SYSTEM_COMMAND_CRYSTAL_OR_TREASURE, 0, source->battle_data->misc_unit_id, 0, 0);
        battle_gfx_prepare_post_action_display_by_misc_id(source->unit_id);
        battle_unit_update_display_by_misc_id(source->unit_id);
        return 1;
    }
    if (g_move_find_result & MOVE_POST_EVENT_MOUNT_STATUS_CHANGE) {
        battle_menu_init_system_function(
            BATTLE_MENU_SYSTEM_COMMAND_CRYSTAL_OR_TREASURE, 0, source->battle_data->misc_unit_id, 0, 0);
        if (source->mount_state != BATTLE_MISC_MOUNT_STATE_NONE) {
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
    if (map_refresh_deep_dungeon() == 0) {
        battle_state_enter_after_command();
    }
    return 0;
}
