#include "fft/open.h"
#include "psx/pad.h"

/* Handler 11, a title demo movie: Start stops the stream. Once the stream has
 * stopped, by itself or on the update after Start, the display is blanked and the
 * controller is replaced by the FFTST.STR logo movie, which leads back to the
 * title menu. */
void open_movie_start_fftst_or_skip_controller(open_controller_record_t* unused_record) {
    if ((g_open_system_runtime_flags & OPEN_SYSTEM_RUNTIME_FLAG_MOVIE_STREAMING) == 0) {
        SetDispMask(0);
        g_active_graphics_buffer_index = 1;
        g_open_system_runtime_flags |= OPEN_SYSTEM_RUNTIME_FLAG_DISPLAY_BLANKED;
        g_open_current_controller_index--;
        open_movie_stream_fftst_and_push_controller(DISC_SECTOR_OPEN_FFTST_STR, 0x278a, 0x3df, 0x2de);
    } else if ((g_open_input_new_button_presses & PSX_PAD_START) != 0) {
        open_movie_stop_stream(1);
    }
}
