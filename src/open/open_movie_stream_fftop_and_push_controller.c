#include "fft/open.h"
#include "psx/types.h"

/* Start the opening movie (the one caller passes the sector of OPEN/FFTOP.STR) and push its controller. */
void open_movie_stream_fftop_and_push_controller(s32 start_sector, s32 unused_stream_length, s32 end_frame) {
    s32 controller;

    (void)unused_stream_length;
    open_movie_start_stream(start_sector, end_frame, -1, 0x94);
    g_open_system_runtime_flags &= ~OPEN_SYSTEM_RUNTIME_FLAG_DISPLAY_BLANKED;
    controller = g_open_current_controller_index;
    g_open_controller_handler_indices[controller] = OPEN_CONTROLLER_FFTOP_MOVIE;
    g_open_current_controller_index = controller + 1;
}

/* The stream plays its CD audio at mix level 0x94. */
