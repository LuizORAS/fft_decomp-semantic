#include "fft/open.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Handler 9, FFTEND.STR, the ending movie: Start stops it. Once it has stopped the display is blanked and
 * the controller is replaced by the ending's credits script (open_script_push_controller). */
void open_movie_update_fftend_controller(void) {
    u32 flags;
    s32 controller;

    if (!(g_open_system_runtime_flags & OPEN_SYSTEM_RUNTIME_FLAG_MOVIE_STREAMING)) {
        SetDispMask(0);
        flags = g_open_system_runtime_flags;
        controller = g_open_current_controller_index;
        flags |= OPEN_SYSTEM_RUNTIME_FLAG_DISPLAY_BLANKED;
        controller--;
        g_open_system_runtime_flags = flags;
        g_open_current_controller_index = controller;
        open_script_push_controller();
        return;
    }

    if (g_open_input_new_button_presses & PSX_PAD_START) {
        open_movie_stop_stream(1);
    }
}
