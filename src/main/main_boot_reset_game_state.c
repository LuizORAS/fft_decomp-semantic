#include "fft/main.h"

/* Start a new game session before each title screen: initialize the save options, status checks,
 * data tables and item order. Every call after the first one (a reset or the end of the game)
 * also frees the game heap, unloads the scenario music, zeroes the play time, restarts the GPU,
 * SPU and CD and reloads the generic sound effects and the zodiac frame under the Squaresoft
 * logo, which stays at least 60 frames; the first call needs none of that, since
 * main_boot_run_startup has just shown the logos. */
void main_boot_reset_game_state(void) {
    int frame;

    if (g_main_boot_game_state_initialized != 0) {
        main_heap_clear_game_allocator_table();
        main_sound_unload_scenario_music_and_stop_tune();
        g_main_system_play_time_hours = 0;
        g_main_system_play_time_minutes = 0;
        g_main_system_play_time_seconds = 0;
        g_main_system_play_time_frames = 0;
        ResetGraph(1);
        SetGraphDebug(0);
        SpuInitHot();
        main_file_init_cd();
        main_gfx_reset_display(256, 240, 512, 0, 0, 0);
        main_boot_fade_in_squaresoft_logo();
        main_sound_open_generic_sfx();
        main_gfx_load_zodiac_frame();
    }

    main_save_init_state_and_options();
    main_status_init_check_data();
    main_save_init_data_tables();
    main_item_init_order_tables();

    if (g_main_boot_game_state_initialized != 0) {
        for (frame = 0; frame < 60; frame++) {
            VSync(0);
        }
        main_boot_fade_out_squaresoft_logo();
    }

    g_main_boot_game_state_initialized = 1;
}
