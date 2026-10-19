#include "fft/battle_script.h"
#include "fft/event_option.h"
#include "fft/script.h"
#include "fft/thread.h"
#include "psx/types.h"

/* Run one of OPTION.OUT's three menus (option_menu_type_e) in this menu thread, whose parameter 1 is its
 * menu record. Unpack g_main_game_options into g_option_current_values; the last value
 * (GAME_OPTION_UNCHANGED) is 1 while every option equals g_option_reference_values. Except for the crystal
 * panel, stop the four unit panels. Then run the game options (option_menu_run_options_thread in the slot
 * below, waiting for it), the AT list (option_menu_start_at_list_thread) or the crystal or chest panel
 * (option_menu_start_crystal_pickup_threads). BATTLE's battle_menu_open_option_* threads start it after loading
 * the overlay; world_option_run_menu is WORLD's copy. */
void option_entrypoint(s32 menu_type) {
    world_menu_entry_t* menu = (world_menu_entry_t*)battle_thread_get_current_parameter_1();
    s32 index;
    game_options_fields_t options = g_main_game_options.fields;

    g_option_current_values[GAME_OPTION_CURSOR_MOVEMENT] = options.cursor_movement;
    g_option_current_values[GAME_OPTION_CURSOR_REPEAT_SPEED] = options.cursor_repeat_speed;
    g_option_current_values[GAME_OPTION_MULTI_HEIGHT_CURSOR_SPEED] = options.multi_height_cursor_speed;
    g_option_current_values[GAME_OPTION_FINGER_CURSOR_REPEAT_SPEED] = options.finger_cursor_repeat_speed;
    g_option_current_values[GAME_OPTION_MESSAGE_DISPLAY_SPEED] = options.message_display_speed;
    g_option_current_values[GAME_OPTION_NAVIGATION_MESSAGES] = options.navigation_messages;
    g_option_current_values[GAME_OPTION_ABILITY_NAMES] = options.ability_names;
    g_option_current_values[GAME_OPTION_EFFECT_MESSAGES] = options.effect_messages;
    g_option_current_values[GAME_OPTION_DISPLAY_GAINED_EXP_JP] = options.display_gained_exp_jp;
    g_option_current_values[GAME_OPTION_TARGET_FLASHING] = options.target_flashing;
    g_option_current_values[GAME_OPTION_SHOW_UNEQUIPPABLE_ITEMS] = options.show_unequippable_items;
    g_option_current_values[GAME_OPTION_MAX_EQUIP_AT_JOB_CHANGE] = options.max_equip_at_job_change;
    g_option_current_values[GAME_OPTION_SOUND_MODE] = options.sound_mode;
    g_option_current_values[GAME_OPTION_UNCHANGED] = 1;

    for (index = 0; index < GAME_OPTION_COUNT; index++) {
        if (g_option_current_values[index] != g_option_reference_values[index]) {
            g_option_current_values[GAME_OPTION_UNCHANGED] = 0;
        }
    }

    if (menu_type != OPTION_MENU_CRYSTAL_PICKUP) {
        thread_set_parameters(THREAD_SLOT_TILE_INFO_PANEL, 0, 0, 1);
        thread_set_parameters(THREAD_SLOT_UNIT_SUMMARY_PANEL, 0, 0, 1);
        thread_set_parameters(THREAD_SLOT_RIGHT_UNIT_STATS, 0, 0, 1);
        thread_set_parameters(THREAD_SLOT_HOVERED_UNIT_STATS, 0, 0, 1);
        thread_wait_until_inactive(THREAD_SLOT_TILE_INFO_PANEL);
        thread_wait_until_inactive(THREAD_SLOT_UNIT_SUMMARY_PANEL);
        thread_wait_until_inactive(THREAD_SLOT_RIGHT_UNIT_STATS);
        thread_wait_until_inactive(THREAD_SLOT_HOVERED_UNIT_STATS);
    }

    g_battle_menu_modal_open = 0;
    g_option_menu_state = 0;
    if (menu_type == OPTION_MENU_GAME_OPTIONS) {
        s32 child_thread;

        script_pulse_tutorial_wait_value(0xfd);
        thread_start(g_thread_current_id - 1, option_menu_run_options_thread);
        child_thread = g_thread_current_id;
        menu->select_text_table = 1;
        menu->context = g_option_menu_entry_table;
        thread_set_parameters(child_thread - 1, menu, 0, 0);
        thread_wait_until_inactive(g_thread_current_id - 1);
        thread_exit_current();
    }
    if (menu_type == OPTION_MENU_AT_LIST) {
        script_pulse_tutorial_wait_value(0xf9);
        option_menu_start_at_list_thread();
    }
    if (menu_type == OPTION_MENU_CRYSTAL_PICKUP) {
        option_menu_start_crystal_pickup_threads();
    }
}
