#include "fft/wldcore.h"
#include "psx/types.h"

/* Pushes the screen transition (level SCREEN_TRANSITION) to the full-screen level `screen`.
 *
 * Clears the screen, sets system flags 0x3800 and starts a 16-step fade,
 * then runs the per-screen teardown for screens 0x1b..0x2d (jump table at
 * 0x80067210; 0x1e..0x2a do nothing):
 *   0x1b  closes the parent list, loads chapter 3, waits for the file, drains
 *         the sound queue, rebalances the volume and restarts music 0x220;
 *   0x1c  hides the render objects and restarts music 0x220;
 *   0x1d  hides the render objects, sets menu slot 4's cursor to 2, restarts
 *         music 0x220, resets the level's window, records in g_wldcore_brave_story_saved_cursor
 *         whether script variable 0x92 and any of variables 0x1a4..0x1bb are
 *         set, closes the parent list, loads chapter 4 and saves the word
 *         pair at 0x80049a10;
 *   0x2b  hides the render objects and restarts music 0x21a;
 *   0x2c  rebuilds the tutorial category panel image and closes the parent
 *         list frame windows;
 *   0x2d  hides the render objects, sets menu slot 4's cursor to 3, resets
 *         the level's window, reopens the tutorial categories and saves the
 *         word pair at 0x80049a10.
 * It then records the three arguments on the new level, zeroes the 0x1e000
 * scratch buffer, blacks the menu brightness out, starts WORLD thread 4 and
 * pushes the level as type 0x31 with delay = 0x14.
 *
 * The new level's fields go through the stack-record union, whose field
 * access is what keeps the target's per-field `$at` bases. */
void wldcore_menu_push_screen_transition_level(s32 screen, s32 param_a, s32 param_b) {
    u32* clear;
    s32 count;
    s32 i;
    s32 depth;

    wldcore_clear_screen_and_set_map_clip_rect(1);
    g_main_system_flags |= MAIN_SYSTEM_FLAG_HIDE_CONTEXT_VALUE | MAIN_SYSTEM_FLAG_HIDE_CALENDAR_DATE
        | MAIN_SYSTEM_FLAG_HIDE_LOCATION_LABEL;
    wldcore_fade_start_screen(0, 0x10);
    switch (screen) {
    case WLDCORE_MENU_LEVEL_BRAVE_STORY:
        wldcore_window_set_render_objects_visible(0);
        wldcore_sound_wait_for_queue_drain();
        wldcore_sound_enqueue_music_start(WLDCORE_AUDIO_MUSIC(2, MAIN_MUSIC_BRAVE_STORY));
        break;
    case WLDCORE_MENU_LEVEL_STORY_EVENTS:
        wldcore_window_set_render_objects_visible(0);
        world_menu_set_slot_4_cursor_index(2);
        count = 0;
        wldcore_sound_wait_for_queue_drain();
        wldcore_sound_enqueue_music_start(WLDCORE_AUDIO_MUSIC(2, MAIN_MUSIC_BRAVE_STORY));
        wldcore_menu_suspend_root_level(&g_wldcore_menu_stack_records[g_wldcore_menu_stack_depth]);
        if (world_script_get_variable(EVENT_SCRIPT_VAR_FACTS_ENABLED) != 0) {
            for (i = 0; i < EVENT_SCRIPT_VAR_FACT_COUNT; i++) {
                if (world_script_get_variable(i + EVENT_SCRIPT_VAR_FACT_FIRST) != 0) {
                    count++;
                }
            }
        }
        if (count != 0) {
            g_wldcore_brave_story_saved_cursor[0] = 1;
        } else {
            g_wldcore_brave_story_saved_cursor[0] = 0;
        }
        wldcore_menu_push_brave_story_level();
        g_main_system_flags |= MAIN_SYSTEM_FLAG_HIDE_CONTEXT_VALUE;
        wldcore_window_close_parent_list_all_windows();
        wldcore_load_message_block_if_changed(WLDCORE_MESSAGE_BLOCK_STORY_EVENTS);
        wldcore_wait_for_file_load();
        g_wldcore_story_events_cursor_state = g_main_saved_list_cursor_state;
        break;
    case WLDCORE_MENU_LEVEL_TUTORIAL_CATEGORIES:
        wldcore_window_set_render_objects_visible(0);
        wldcore_sound_wait_for_queue_drain();
        wldcore_sound_enqueue_music_start(WLDCORE_AUDIO_MUSIC(2, MAIN_MUSIC_TUTORIAL));
        break;
    case WLDCORE_MENU_LEVEL_FORMATION_TUTORIALS:
        wldcore_list_build_tutorial_category_panel_image(
            &g_wldcore_menu_stack_records[g_wldcore_menu_stack_depth].text_list);
        wldcore_window_close_parent_list_frame_windows();
        break;
    case WLDCORE_MENU_LEVEL_BATTLE_TUTORIALS:
        wldcore_window_set_render_objects_visible(0);
        world_menu_set_slot_4_cursor_index(3);
        wldcore_menu_suspend_root_level(&g_wldcore_menu_stack_records[g_wldcore_menu_stack_depth]);
        wldcore_list_open_tutorial_categories(1, 0);
        wldcore_window_close_parent_list_frame_windows();
        g_wldcore_tutorial_entries_cursor_state = g_main_saved_list_cursor_state;
        break;
    case WLDCORE_MENU_LEVEL_TREASURE_DETAIL:
        wldcore_window_close_parent_list_all_windows();
        wldcore_load_message_block_if_changed(WLDCORE_MESSAGE_BLOCK_TREASURES);
        wldcore_wait_for_file_load();
        wldcore_sound_wait_for_queue_drain();
        main_sound_set_master_volume(0x3FFF, 1);
        wldcore_sound_enqueue_music_start(WLDCORE_AUDIO_MUSIC(2, MAIN_MUSIC_BRAVE_STORY));
        break;
    default:
        break;
    }
    i = 0x77FF;
    clear = &g_wldcore_scratch_buffer[0x77FF];
    depth = g_wldcore_menu_stack_depth;
    g_wldcore_menu_stack_records_next[depth].screen_transition.screen = screen;
    g_wldcore_menu_stack_records_next[depth].screen_transition.param_a = param_a;
    g_wldcore_menu_stack_records_next[depth].screen_transition.param_b = param_b;
    for (; i >= 0; i--) {
        *clear = 0;
        clear--;
    }
    world_menu_set_brightness(0, 0, 0);
    world_thread_start(WLDCORE_THREAD_SLOT_TRANSITION, world_menu_zodiac_backdrop_thread);
    world_thread_set_parameters(WLDCORE_THREAD_SLOT_TRANSITION, 0, 0, 0);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].screen_transition.delay = 0x14;
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].screen_transition.phase = 0;
    g_wldcore_menu_stack_types[g_wldcore_menu_stack_depth + 1] = WLDCORE_MENU_LEVEL_SCREEN_TRANSITION;
    g_wldcore_menu_stack_depth++;
}
