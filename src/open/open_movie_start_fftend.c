#include "fft/open.h"

/* Start FFTEND.STR (sector 0x21908, ending at frame 0x571) as the first controller of the ending. */
void open_movie_start_fftend(void) {
    g_open_current_controller_index = 0;
    g_active_graphics_buffer_index = 0;
    open_movie_push_fftend_controller(DISC_SECTOR_OPEN_FFTEND_STR, 0x3858, 0x571);
}
