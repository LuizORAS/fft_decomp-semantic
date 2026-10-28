#include "fft/open.h"

/* Set up the ending for open_system_run_ending_cutscene: unload the scenario music, reset the runtime
 * state, save the 64x256 VRAM page at (960, 256) to (960, 0) (open_gfx_restore_frame_vram_page puts it back),
 * load the ending's graphics, build the screen environments and the render records and start FFTEND.STR. */
void open_system_init_ending(void) {
    RECT source;

    main_sound_unload_scenario_music_and_stop_tune();
    open_system_init_runtime_state();

    source.x = 0x3c0;
    source.y = 0x100;
    source.w = 0x40;
    source.h = 0x100;
    MoveImage(&source, 0x3c0, 0);
    DrawSync(0);

    g_open_system_vsync_mode = 0;
    open_file_load_end_movie_graphics();
    open_gfx_init_screen_environments(1);
    open_gfx_init_render_buffers();
    open_movie_start_fftend();
}
