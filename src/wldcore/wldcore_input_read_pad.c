#include "fft/wldcore.h"
#include "psx/pad.h"
#include "psx/types.h"

/* WLDCORE's pad read, once a frame from the frame draws: reads pad 0 and publishes the held buttons
 * and the newly pressed ones (both 0 while MAIN_SYSTEM_FLAG_INPUT_BLOCKED is set). Holding exactly
 * L1 + R1 + Select + Start (MAIN_INPUT_RESET_BUTTONS) restores the sound volumes and resets the
 * game. */
void wldcore_input_read_pad(void) {
    u32 input;
    u32 previous;

    g_wldcore_previous_button_input = g_wldcore_polled_button_input;
    input = PadRead(0);
    g_wldcore_polled_button_input = input;

    if (g_main_system_flags & MAIN_SYSTEM_FLAG_INPUT_BLOCKED) {
        g_wldcore_new_button_presses = 0;
        g_wldcore_current_button_input = 0;
    } else {
        previous = g_wldcore_previous_button_input;
        g_wldcore_current_button_input = input;
        g_wldcore_new_button_presses = input & ~previous;
    }

    if (g_wldcore_polled_button_input == MAIN_INPUT_RESET_BUTTONS) {
        DrawSync(0);
        main_sound_set_master_volume(0x3fff, 1);
        main_sound_put_type(0xc0);
        main_sound_set_cd_volume(0x74ff, 1);
        main_system_reset_game();
    }
}
