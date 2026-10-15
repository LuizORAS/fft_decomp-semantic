#include "fft/world.h"

/* Run the formation screen an event asked for. When EVENT_SCRIPT_VAR_FORMATION_RETURN_EVENT
 * is set, WLDCORE's exit (wldcore_system_run_requested_formation) calls this instead of leaving: it unloads the
 * scenario music, clears the screen, reloads the menu text with the face images and the input timing
 * of the game options, runs world_formation_run_screen, then makes the stored event current
 * (EVENT_SCRIPT_VAR_CURRENT_EVENT) and clears that variable and EVENT_SCRIPT_VAR_NEXT_SCENARIO. */
void world_formation_run_before_event(void) {
    s32 profile;

    main_sound_unload_scenario_music_and_stop_tune();
    ClearImage(&g_world_screen_clear_rect, 0, 0, 0);
    DrawSync(0);
    VSync(0);
    world_menu_init_with_world_text_and_faces();
    DrawSync(0);
    VSync(0);
    world_menu_reset_context();
    DrawSync(0);
    VSync(0);
    profile = g_main_game_options.fields.finger_cursor_repeat_speed;
    g_main_input_repeat_initial_delay = g_world_input_timing_profiles[profile].initial_delay;
    g_main_input_repeat_period = g_world_input_timing_profiles[profile].repeat_period;
    g_main_input_secondary_repeat_period = g_world_input_timing_profiles[profile].secondary_repeat_period;
    g_main_menu_scroll_accel_delay = g_world_input_timing_profiles[profile].scroll_accel_delay;
    g_main_menu_scroll_slow_step = g_world_input_timing_profiles[profile].scroll_slow_step;
    g_main_menu_scroll_fast_step = g_world_input_timing_profiles[profile].scroll_fast_step;
    world_menu_set_brightness(0x80, 0x80, 0x80);
    world_formation_run_screen(0, 0);
    world_menu_reset_context();
    ClearImage(&g_world_screen_clear_rect, 0, 0, 0);
    DrawSync(0);
    world_script_set_variable(
        EVENT_SCRIPT_VAR_CURRENT_EVENT, world_script_get_variable(EVENT_SCRIPT_VAR_FORMATION_RETURN_EVENT));
    world_script_set_variable(EVENT_SCRIPT_VAR_FORMATION_RETURN_EVENT, 0);
    if (world_script_get_variable(EVENT_SCRIPT_VAR_NEXT_SCENARIO) != 0) {
        world_script_set_variable(EVENT_SCRIPT_VAR_NEXT_SCENARIO, 0);
    }
}
