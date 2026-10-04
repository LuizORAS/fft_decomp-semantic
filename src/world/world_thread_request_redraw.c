#include "fft/world.h"

/* Set thread_id's task_words[4], which world_menu_redraw_text_page_on_scroll reads as a
 * request to redraw its page; WLDCORE's send-unit screens send it to thread 12. */
void world_thread_request_redraw(s32 thread_id) {
    g_world_threads[thread_id].task_words[4] = 1;
}
