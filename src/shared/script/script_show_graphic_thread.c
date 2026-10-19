#include "fft/event_etc.h"
#include "fft/menu.h"
#include "fft/script.h"
#include "fft/thread.h"

/* Thread body of ShowGraphic (EVENT_OPCODE_SHOW_GRAPHIC): open the ETC overlay and run its graphic
 * display on this thread. */
void script_show_graphic_thread(void) {
    thread_set_current_task_id(THREAD_TASK_SHOW_GRAPHIC);
    menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_ETC);
    etc_graphic_show_async();
}
