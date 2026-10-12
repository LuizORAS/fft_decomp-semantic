#include "fft/wldcore.h"
#include "psx/pad.h"

typedef struct wldcore_deep_dungeon_battle {
    u8 map_id;   /* 0x00 */
    u8 squad_id; /* 0x01: deployment zone id */
    u8 choices[8];
} wldcore_deep_dungeon_battle_t;

extern wldcore_deep_dungeon_battle_t* g_wldcore_deep_dungeon_battles;

s32 rand(void);

/* Step of a location's menu level: fade its windows in, start the list panel thread after 8 frames,
 * then handle the input. Circle on an entry fades the screen and the music out and closes the panel,
 * then:
 *  - the Bar opens with its music (wldcore_bar_push_menu_level);
 *  - the Shop, Soldier Office and Fur Shop run that screen (wldcore_menu_run_shop_and_rebuild_screen);
 *  - a Deep Dungeon floor runs the location's WLDCORE_SCRIPT_ACTION_DEEP_DUNGEON_ENTRY condition:
 *    when it holds its battle starts (the record's map and deployment squad, one of its eight ENTDs
 *    at random, the event, a random exit), otherwise the map fades back in;
 *  - an event choice starts its event on its map.
 * X closes the menu, and Select shows the entry's help text. */
void wldcore_menu_step_location_menu_level(wldcore_menu_location_menu_level_t* level) {
    s16* selected;
    u8 unused[0x28];
    s16* values;
    s32 value;
    s32 text;
    s32 offset;
    wldcore_deep_dungeon_battle_t* records;
    wldcore_deep_dungeon_battle_t* record;

    if (level->fade != 0) {
        if (level->fade >= 0x80) {
            level->fade = 0x80;
        }
        g_wldcore_window_record_colors[level->slots.window_a].red = level->fade;
        g_wldcore_window_record_colors[level->slots.window_a].green = level->fade;
        g_wldcore_window_record_colors[level->slots.window_a].blue = level->fade;
        g_wldcore_window_record_colors[level->slots.window_b].red = level->fade;
        g_wldcore_window_record_colors[level->slots.window_b].green = level->fade;
        g_wldcore_window_record_colors[level->slots.window_b].blue = level->fade;
        if (level->fade == 0x80) {
            level->fade = 0;
            wldcore_gfx_toggle_captured_world_frame(1);
        } else {
            level->fade += 7;
        }
        return;
    }
    if (level->delay != 0) {
        if (level->delay == 8) {
            world_thread_start(WLDCORE_THREAD_SLOT_WINDOW_PANEL, world_screen_build_at_list);
            world_thread_set_parameters(
                WLDCORE_THREAD_SLOT_WINDOW_PANEL, (s32)&g_wldcore_window_panel_render_state, 0, 0);
        }
        level->delay--;
        return;
    }
    if (g_main_system_flags & MAIN_SYSTEM_FLAG_FADING) {
        return;
    }
    if ((g_main_system_flags & MAIN_SYSTEM_FLAG_INPUT_BLOCKED)
        && world_thread_is_running(WLDCORE_THREAD_SLOT_WINDOW_PANEL) == 0) {
        wldcore_gfx_toggle_captured_world_frame(0);
        g_main_system_flags = (g_main_system_flags | MAIN_SYSTEM_FLAG_INPUT_BLOCKED)
            & ~(MAIN_SYSTEM_FLAG_DRAW_MAP | MAIN_SYSTEM_FLAG_FREEZE_ANIMATION);
        if (level->slots.result != 0) {
            wldcore_list_store_cursor_state(WLDCORE_CURSOR_SLOT_LOCATION_MENU);
            selected = &g_wldcore_window_panel_render_state.selected_index;
            values = g_wldcore_list_entry_values;
            value = values[*selected];
            if ((value & TEXT_ID_SECTION_MASK) == TEXT_ID_WORLD_MENU_BASE) {
                g_main_system_flags ^= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
                value &= TEXT_ID_ENTRY_MASK;
                switch (value) {
                case WLDCORE_TEXT_ID_BAR& TEXT_ID_ENTRY_MASK:
                    wldcore_menu_close_level_and_release_records(&level->slots, 0);
                    wldcore_sound_enqueue_music_start(WLDCORE_AUDIO_MUSIC(2, MAIN_MUSIC_BAR));
                    wldcore_bar_push_menu_level();
                    return;
                case WLDCORE_TEXT_ID_SHOP& TEXT_ID_ENTRY_MASK:
                    wldcore_menu_close_level_and_release_records(&level->slots, 0);
                    wldcore_menu_run_shop_and_rebuild_screen(0);
                    return;
                case WLDCORE_TEXT_ID_SOLDIER_OFFICE& TEXT_ID_ENTRY_MASK:
                    wldcore_menu_close_level_and_release_records(&level->slots, 0);
                    wldcore_menu_run_shop_and_rebuild_screen(WORLD_SHOP_ID_SOLDIER_OFFICE);
                    return;
                case WLDCORE_TEXT_ID_FUR_SHOP& TEXT_ID_ENTRY_MASK:
                    wldcore_menu_close_level_and_release_records(&level->slots, 0);
                    wldcore_menu_run_shop_and_rebuild_screen(WORLD_SHOP_ID_FUR_SHOP);
                    return;
                default:
                    world_script_set_variable(EVENT_SCRIPT_VAR_DEEP_DUNGEON_SELECTION,
                        value - (WLDCORE_TEXT_ID_DEEP_DUNGEON_FLOOR_FIRST & TEXT_ID_ENTRY_MASK));
                    wldcore_menu_close_level_and_release_records(&level->slots, 0);
                    if (wldcore_script_process_conditional_set(
                            g_wldcore_map_projection_state.marker.kind, WLDCORE_SCRIPT_ACTION_DEEP_DUNGEON_ENTRY)
                        != 0) {
                        offset = g_wldcore_script_state.args[0] * 10;
                        records = g_wldcore_deep_dungeon_battles;
                        /* The byte offset and shared base preserve the target's choice-index calculation. */
                        record = (wldcore_deep_dungeon_battle_t*)((u8*)records + offset);
                        g_wldcore_next_map_id[0] = record->map_id;
                        world_script_set_variable(
                            EVENT_SCRIPT_VAR_CURRENT_ENTD, ((u8*)records)[((rand() * 8) >> 15) + offset + 2]);
                        world_script_set_variable(EVENT_SCRIPT_VAR_DEPLOYMENT_SQUAD_COUNT, 1);
                        world_script_set_variable(EVENT_SCRIPT_VAR_DEPLOYMENT_PRIMARY_SQUAD_ID, record->squad_id);
                        world_script_set_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT, g_wldcore_script_state.args[1]);
                        world_script_set_variable(
                            EVENT_SCRIPT_VAR_DEEP_DUNGEON_EXIT, (rand() * g_wldcore_script_state.args[2]) >> 15);
                        g_main_system_flags
                            = (g_main_system_flags | MAIN_SYSTEM_FLAG_DEEP_DUNGEON_BATTLE) ^ MAIN_SYSTEM_FLAG_RUNNING;
                    } else {
                        wldcore_fade_start_screen(0, 0x10);
                        wldcore_sound_enqueue_music_start(WLDCORE_AUDIO_MUSIC(1, MAIN_MUSIC_WORLD_MAP));
                    }
                    return;
                }
            }
            world_script_set_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT, g_wldcore_script_state.args[*selected]);
            g_main_system_flags ^= MAIN_SYSTEM_FLAG_RUNNING;
            value = values[*selected] & TEXT_ID_ENTRY_MASK;
            g_wldcore_next_map_id[0] = value + 1;
            return;
        }
        g_main_system_flags = (g_main_system_flags | MAIN_SYSTEM_FLAG_DRAW_MAP) & ~MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
        wldcore_list_clear_cursor_state(WLDCORE_CURSOR_SLOT_LOCATION_MENU);
        wldcore_menu_close_level_and_release_records(&level->slots, 1);
        return;
    }
    if (g_wldcore_new_button_presses & PSX_PAD_CROSS) {
        value = g_wldcore_list_entry_values[g_wldcore_window_panel_render_state.selected_index];
        if ((value & TEXT_ID_SECTION_MASK) == TEXT_ID_WORLD_MENU_BASE) {
            wldcore_sound_play_effect(MAIN_SFX_CANCEL);
            level->slots.result = 0;
            world_thread_set_parameters(WLDCORE_THREAD_SLOT_WINDOW_PANEL, 0, 0, 1);
            g_main_system_flags |= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
        }
    } else if (g_wldcore_new_button_presses & PSX_PAD_SELECT) {
        wldcore_window_set_render_state_2_from_arg_0x10((s32*)level);
        value = g_wldcore_list_entry_values[g_wldcore_window_panel_render_state.selected_index];
        if ((value & TEXT_ID_SECTION_MASK) == TEXT_ID_MAP_NAME_BASE) {
            /* Map name n: WLDHELP.LZW 0x8801 + n describes the map. */
            wldcore_menu_push_message_level((value & TEXT_ID_ENTRY_MASK) + 0x8801, 1);
        } else {
            /* Bar/Town entry n: its WLDHELP.LZW help is 0x1000 + n - 0x12 (Bar
             * 0x5d -> 0x104b), and 0x1000 + n - 0x5d from entry 0xed on. */
            value &= TEXT_ID_ENTRY_MASK;
            text = value + 0xFEE;
            if (value >= (WLDCORE_TEXT_ID_DEEP_DUNGEON_FLOOR_FIRST & TEXT_ID_ENTRY_MASK)) {
                text = value + 0xFA3;
            }
            wldcore_menu_push_message_level(text, 1);
        }
    } else if ((g_wldcore_new_button_presses & PSX_PAD_CIRCLE) && level->entry_count != 0
        && g_main_file_still_loading == 0) {
        wldcore_fade_start_screen(WLDCORE_SCREEN_FADE_OUT, 0x10);
        wldcore_sound_enqueue_audio_command(WLDCORE_AUDIO_COMMAND_FADE_OUT, 0x10);
        wldcore_sound_enqueue_audio_command(WLDCORE_AUDIO_COMMAND_UNLOAD, 2);
        wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
        level->slots.result = 1;
        world_thread_set_parameters(WLDCORE_THREAD_SLOT_WINDOW_PANEL, 0, 0, 1);
        g_main_system_flags |= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
    }
}
