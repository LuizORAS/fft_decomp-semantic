#include "fft/thread.h"
#include "fft/world.h"
#include "psx/pad.h"

/* Shop step EXIT_FADE: once the goodbye line shows, any face button starts the screen and
 * music fade-out; when the fade ends the step becomes WORLD_SHOP_STEP_EXIT, which ends
 * world_shop_run_screen. The gil window (and the soldier office's fee window) stays drawn. */
void world_shop_run_exit_fade_step(void) {
    if (g_world_shop_exit_step_initialized == 0) {
        g_world_shop_exit_fade_started = 0;
        g_world_shop_exit_step_initialized = 1;
    }
    if (g_world_shop_exit_fade_started != 0) {
        if (world_gfx_update_fade_out_tile() == 0) {
            g_world_shop_menu_step = WORLD_SHOP_STEP_EXIT;
            g_world_shop_exit_step_initialized = 0;
        }
    } else if (g_world_input_newly_pressed & (PSX_PAD_TRIANGLE | PSX_PAD_CIRCLE | PSX_PAD_CROSS | PSX_PAD_SQUARE)) {
        world_gfx_start_increasing_fade();
        main_sound_set_current_music_target(0, 0xF0);
        thread_set_parameters(THREAD_SCREEN_SLOT_MESSAGE, 0x19, -1, 0);
        g_world_shop_exit_fade_started = 1;
    }
    world_shop_run_gil_menu_script();
    if (g_world_menu_context_id == WORLD_MENU_CONTEXT_SOLDIER_OFFICE) {
        world_shop_run_soldier_office_fee_menu_script();
        world_menu_run_script_with_callback_suppressed();
    }
}
