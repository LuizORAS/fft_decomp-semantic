#include "fft/open.h"

/* Start the logo movie at CD mix level 0x76 and push handler 0, whose record
 * keeps the stream's first sector and length for the title menu's CD end
 * position. */
void open_movie_stream_fftst_and_push_controller(
    s32 start_sector, s32 stream_length, s32 end_frame, s32 video_end_frame) {
    s32 controller;

    open_movie_start_stream(start_sector, end_frame, video_end_frame, 0x76);
    g_open_system_runtime_flags &= ~OPEN_SYSTEM_RUNTIME_FLAG_DISPLAY_BLANKED;
    controller = g_open_current_controller_index;
    g_open_controller_records_next[controller].movie.start_sector = start_sector;
    g_open_controller_records_next[controller].movie.stream_length = stream_length;
    g_open_controller_handler_indices[controller] = OPEN_CONTROLLER_FFTST_MOVIE;
    g_open_current_controller_index = controller + 1;
}
