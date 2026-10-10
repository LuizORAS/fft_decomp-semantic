#include "fft/open.h"
#include "psx/types.h"

/* First controller of the title sequence: the FFTST.STR logo movie (sector
 * 0x153d6, 0x278a sectors; the stream ends at frame 0x3df and the video alone at
 * frame 0x2de), or the title menu at once when use_alternate is set. */
void open_movie_start_fftst_or_title_menu(s32 use_alternate) {
    g_open_current_controller_index = 0;
    if (use_alternate == 0) {
        g_active_graphics_buffer_index = 0;
        open_movie_stream_fftst_and_push_controller(DISC_SECTOR_OPEN_FFTST_STR, 0x278A, 0x3DF, 0x2DE);
    } else {
        open_title_push_menu_controller(0);
    }
}
