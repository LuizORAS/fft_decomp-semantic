#include "fft/event_etc.h"
#include "fft/menu.h"
#include "fft/script.h"
#include "fft/thread.h"

/* Open the ETC overlay and run its graphic display under the graphic task id. */
void script_show_graphic(void) {
    thread_set_current_task_id(NATIVE_THREAD_TASK_SHOW_GRAPHIC);
    menu_request_open_companion_executable(COMPANION_OVERLAY_ETC);
    etc_graphic_show_async();
}
