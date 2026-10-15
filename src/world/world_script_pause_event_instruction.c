#include "fft/world.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Pause (EVENT_OPCODE_PAUSE): yield until Select, Cross or Circle is pressed, or until the thread's
 * task id becomes NATIVE_THREAD_TASK_STOP_REQUEST. */
void world_script_pause_event_instruction(void) {
    do {
        world_thread_yield();
    } while (world_thread_get_current_task_id() != NATIVE_THREAD_TASK_STOP_REQUEST
        && (g_world_menu_new_button_input & (PSX_PAD_CIRCLE | PSX_PAD_CROSS | PSX_PAD_SELECT)) == 0);
}
