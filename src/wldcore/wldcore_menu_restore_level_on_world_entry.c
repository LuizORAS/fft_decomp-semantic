#include "fft/script.h"
#include "fft/wldcore.h"

/* Clear MAIN_SYSTEM_FLAG_RESTART and open what the visit starts with. Normally that is the
 * current location's arrival text (its WLDCORE_SCRIPT_ACTION_TEXT condition, a world help message in
 * the scrollable text window) or else its pending map requests
 * (wldcore_menu_dispatch_pending_script_requests), with the world map music fading in. A replayed
 * story event returns to the Story Events list and a tutorial event to its tutorial list (both
 * restoring the weather from g_main_saved_weather_variable; the tutorial also its music), and the
 * title's Tutorial and Brave Story choices open those screens
 * (wldcore_menu_push_screen_transition_level). */
void wldcore_menu_restore_level_on_world_entry(void) {
    s32 flags;

    flags = g_main_system_flags & ~MAIN_SYSTEM_FLAG_RESTART;
    g_main_system_flags = flags;
    if ((flags
            & (MAIN_SYSTEM_FLAG_STORY_EVENT_REPLAY | MAIN_SYSTEM_FLAG_TITLE_TUTORIAL | MAIN_SYSTEM_FLAG_TUTORIAL_EVENT
                | MAIN_SYSTEM_FLAG_TITLE_BRAVE_STORY))
        == 0) {
        if (wldcore_script_find_location_request(g_wldcore_map_projection_state.marker.kind, WLDCORE_SCRIPT_ACTION_TEXT)
            != 0) {
            wldcore_menu_suspend_root_level(&g_wldcore_menu_stack_records[g_wldcore_menu_stack_depth]);
            wldcore_menu_push_scrollable_text_window_level(
                g_wldcore_script_state.args[0] + TEXT_ID_WORLD_HELP_MESSAGE_BASE);
        } else {
            wldcore_menu_dispatch_pending_script_requests();
        }
        wldcore_sound_enqueue_audio_command(
            WLDCORE_AUDIO_COMMAND_PLAY_MUSIC, WLDCORE_AUDIO_MUSIC(1, MAIN_MUSIC_WORLD_MAP));
        wldcore_sound_enqueue_audio_command(WLDCORE_AUDIO_COMMAND_FADE_IN, 4);
        return;
    }
    if (flags & MAIN_SYSTEM_FLAG_STORY_EVENT_REPLAY) {
        g_main_system_flags = flags ^ MAIN_SYSTEM_FLAG_STORY_EVENT_REPLAY;
        script_set_variable(EVENT_SCRIPT_VAR_WEATHER, g_main_saved_weather_variable);
        wldcore_menu_push_screen_transition_level(WLDCORE_MENU_LEVEL_STORY_EVENTS, 0, 0);
    } else if (flags & MAIN_SYSTEM_FLAG_TUTORIAL_EVENT) {
        g_main_system_flags = flags ^ MAIN_SYSTEM_FLAG_TUTORIAL_EVENT;
        script_set_variable(EVENT_SCRIPT_VAR_WEATHER, g_main_saved_weather_variable);
        g_wldcore_audio_queue.music.tracks[0] = 0x1B;
        g_wldcore_audio_queue.music.tracks[1] = 0x1A;
        g_wldcore_audio_queue.current_slot = 2;
        wldcore_menu_push_screen_transition_level(WLDCORE_MENU_LEVEL_BATTLE_TUTORIALS, 1, 0);
    } else if (flags & MAIN_SYSTEM_FLAG_TITLE_TUTORIAL) {
        wldcore_menu_push_screen_transition_level(WLDCORE_MENU_LEVEL_TUTORIAL_CATEGORIES, 0, 1);
    } else if (flags & MAIN_SYSTEM_FLAG_TITLE_BRAVE_STORY) {
        wldcore_menu_push_screen_transition_level(WLDCORE_MENU_LEVEL_BRAVE_STORY, 0, 0);
    }
}
