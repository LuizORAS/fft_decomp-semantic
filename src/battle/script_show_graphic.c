#include "fft/battle_script.h"
#include "fft/event_etc.h"

/* Thread body of ShowGraphic: open the ETC overlay and run its graphic display on
 * this thread. */
void script_show_graphic(void) {
    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_SHOW_GRAPHIC);
    battle_menu_request_open_companion_executable(COMPANION_OVERLAY_ETC);
    etc_graphic_show_async();
}
