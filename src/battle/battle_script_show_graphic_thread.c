#include "fft/battle.h"
#include "fft/event_etc.h"
#include "psx/types.h"

/* Thread body of ShowGraphic (EVENT_OPCODE_SHOW_GRAPHIC): open the ETC overlay and run its graphic
 * display on this thread. */
void battle_script_show_graphic_thread(void) {
    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_SHOW_GRAPHIC);
    battle_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_ETC);
    etc_graphic_show_async();
}
