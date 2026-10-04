#include "fft/main.h"
#include "psx/types.h"

/* Cold boot: clear the game allocator, install the VSync, DrawSync and CD callbacks, reset the
 * GPU, pads, SPU and CD, show the SCEA and Squaresoft logos, open the memory card events and
 * generic sound effects, load the zodiac frame, seed rand with 1 and fade the Squaresoft logo
 * out. */
void main_boot_run_startup(void) {
    main_heap_clear_game_allocator_table();
    ResetCallback();

    g_main_system_play_time_hours = 0;
    g_main_system_play_time_minutes = 0;
    g_main_system_play_time_seconds = 0;
    g_main_system_play_time_frames = 0;

    VSyncCallback(main_system_handle_vsync_callback);
    DrawSyncCallback(main_system_handle_draw_sync_callback);
    CdReadyCallback(main_file_handle_cd_ready_callback);
    CdReadCallback(main_file_handle_cd_read_callback);

    ResetGraph(0);
    SetGraphDebug(0);
    PadInit(0);
    SpuInit();
    main_file_reset_cdrom_cpu_ram();
    main_gfx_reset_display(256, 240, 512, 0, 0, 0);
    main_boot_show_sceap_logo();
    main_boot_fade_in_squaresoft_logo();
    main_card_init_events();
    main_sound_open_generic_sfx();
    main_gfx_load_zodiac_frame();
    srand(1);
    main_boot_fade_out_squaresoft_logo();
    g_main_boot_game_state_initialized = 0;
}
