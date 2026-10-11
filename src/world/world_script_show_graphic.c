#include "fft/event_etc.h"
#include "fft/world.h"
#include "psx/types.h"

/* Thread body of the show-graphic event instruction: load the ETC overlay
 * and run its graphic routine, which exits the thread. */
void world_script_show_graphic(void) {
    world_thread_set_current_task_id(NATIVE_THREAD_TASK_SHOW_GRAPHIC);
    world_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_ETC);
    etc_graphic_show_async();
}
