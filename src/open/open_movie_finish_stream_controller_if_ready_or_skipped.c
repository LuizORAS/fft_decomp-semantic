#include "fft/open.h"
#include "psx/pad.h"

/* Handler 0, the FFTST.STR logo movie: when the stream has ended, or on Start
 * (which stops the video, blanks the display and leaves the CD audio playing),
 * the controller is replaced by the title menu. The menu is given the sector just
 * past the stream, from the record's start sector and length, as the place to
 * stop the CD audio. */
void open_movie_finish_stream_controller_if_ready_or_skipped(open_controller_record_t* record) {
    const open_movie_stream_controller_state_t* state = &record->movie;
    if (g_open_system_runtime_flags & OPEN_SYSTEM_RUNTIME_FLAG_MOVIE_STREAMING) {
        if (!(g_open_input_new_button_presses & PSX_PAD_START)) {
            return;
        }

        open_movie_stop_stream(0);
        SetDispMask(0);
        g_open_system_runtime_flags |= OPEN_SYSTEM_RUNTIME_FLAG_DISPLAY_BLANKED;
    }

    g_open_current_controller_index--;
    open_title_push_menu_controller(state->start_sector + state->stream_length);
}
