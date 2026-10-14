#include "fft/open.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Handler 1, FFTOP.STR, the opening movie that follows the birthday: Start stops it. Once it has stopped
 * the controller pops, the display is blanked and OPEN ends with result 0, a new game, which MAIN takes
 * straight into BATTLE. */
void open_movie_update_fftop_controller(void) {
    u32 flags;
    s32 controller;

    if (!(g_open_system_runtime_flags & OPEN_SYSTEM_RUNTIME_FLAG_MOVIE_STREAMING)) {
        SetDispMask(0);
        flags = g_open_system_runtime_flags;
        controller = g_open_current_controller_index;
        g_open_system_result = 0;
        flags |= OPEN_SYSTEM_RUNTIME_FLAG_DISPLAY_BLANKED;
        controller--;
        g_open_system_runtime_flags = flags;
        flags ^= OPEN_SYSTEM_RUNTIME_FLAG_RUNNING;
        g_open_current_controller_index = controller;
        g_open_system_runtime_flags = flags;
        return;
    }

    if (g_open_input_new_button_presses & PSX_PAD_START) {
        open_movie_stop_stream(1);
    }
}
