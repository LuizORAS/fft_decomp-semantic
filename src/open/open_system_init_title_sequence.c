#include "fft/open.h"
#include "psx/types.h"

/* Set up the title sequence for open_system_run_main_loop: unload the scenario music, reset the runtime
 * state, load the title graphics, build the screen environments and the render records, take the pad repeat
 * timings and start FFTST.STR, or the title menu when skip_movie is set. */
void open_system_init_title_sequence(s32 skip_movie) {
    main_sound_unload_scenario_music_and_tunes();
    open_system_init_runtime_state();
    open_file_load_main_menu_graphics();
    open_gfx_init_screen_environments(1);
    open_gfx_init_render_buffers();
    open_input_init_directional_state();
    open_movie_start_fftst_or_title_menu(skip_movie);
}
