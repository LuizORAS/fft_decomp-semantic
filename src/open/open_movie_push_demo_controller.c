#include "fft/open.h"

/* Start a title demo movie at the given CD mix level and push handler 11. The first sector and length
 * are stored in the controller record, which handler 11 does not read. */
void open_movie_push_demo_controller(s32 start_sector, s32 stream_length, s32 end_frame, s32 cd_mix_level) {
    s32 controller;

    open_movie_start_stream(start_sector, end_frame, -1, cd_mix_level);
    g_open_system_runtime_flags &= ~OPEN_SYSTEM_RUNTIME_FLAG_DISPLAY_BLANKED;
    controller = g_open_current_controller_index;
    g_open_controller_records_next[controller].movie.start_sector = start_sector;
    g_open_controller_records_next[controller].movie.stream_length = stream_length;
    g_open_controller_handler_indices[controller] = OPEN_CONTROLLER_TITLE_DEMO_MOVIE;
    g_open_current_controller_index = controller + 1;
}
