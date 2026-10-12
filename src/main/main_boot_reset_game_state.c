#include "fft/main.h"

/* Prepare a fresh session before the opening runs: reset the 32-bit save values and
 * options, rebuild the status check sets and restore the save data and item order
 * tables.
 *
 * On the first pass after power-on only that is done, because
 * main_boot_run_startup has just shown the logos. On every later pass (a soft
 * reset or the end of the game) it first frees the game heap, unloads the
 * scenario music, zeroes the play time, restarts the GPU, SPU and CD, and shows
 * the Squaresoft logo again for at least 60 frames while the shared sounds and
 * the zodiac frame reload. */
void main_boot_reset_game_state(void) {
    int frame;

    if (g_main_boot_startup2_has_run != 0) {
        main_heap_clear_game_allocator_table();
        main_sound_unload_scenario_music_and_tunes();
        g_main_system_play_time_hours = 0;
        g_main_system_play_time_minutes = 0;
        g_main_system_play_time_seconds = 0;
        g_main_system_play_time_frames = 0;
        ResetGraph(1);
        SetGraphDebug(0);
        SpuInitHot();
        main_file_reset_cdrom_cpu_ram();
        main_gfx_reset_display(256, 240, 512, 0, 0, 0);
        main_boot_build_and_draw_squaresoft_logo();
        main_sound_open_generic_sfx();
        main_gfx_load_zodiac_frame();
    }

    main_save_init_state_and_options();
    main_status_init_check_data();
    main_save_init_data_tables();
    main_item_init_order_tables();

    if (g_main_boot_startup2_has_run != 0) {
        for (frame = 0; frame < 60; frame++) {
            VSync(0);
        }
        main_boot_fade_out_squaresoft_logo();
    }

    g_main_boot_startup2_has_run = 1;
}
