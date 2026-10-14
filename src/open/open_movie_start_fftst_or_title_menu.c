#include "fft/open.h"
#include "psx/types.h"

/* First controller of the title sequence: FFTST.STR (sector 0x153d6, 0x278a sectors; the stream ends at
 * frame 0x3df and the video alone at frame 0x2de, leaving the audio to play on), or the title menu at once
 * when skip_movie is set. */
void open_movie_start_fftst_or_title_menu(s32 skip_movie) {
    g_open_current_controller_index = 0;
    if (skip_movie == 0) {
        g_active_graphics_buffer_index = 0;
        open_movie_push_fftst_controller(DISC_SECTOR_OPEN_FFTST_STR, 0x278A, 0x3DF, 0x2DE);
    } else {
        open_title_push_menu_controller(0);
    }
}
