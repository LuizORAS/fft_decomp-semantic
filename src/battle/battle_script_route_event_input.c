#include "fft/battle.h"
#include "psx/types.h"

/* Pick the input a thread sees; battle_thread_yield calls it before it resumes each thread. Under
 * tutorial input (MENU_INPUT_TUTORIAL) a thread running as NATIVE_THREAD_TASK_RESUME (a dialog text
 * thread) gets the real pad input and every other thread the input the tutorial script forced; the
 * main loop's slot is left alone. */
void battle_script_route_event_input(void) {
    s32 thread;

    if (g_battle_menu_input_mode != MENU_INPUT_TUTORIAL) {
        return;
    }
    thread = g_battle_thread_current_id;
    if (thread == NATIVE_THREAD_SLOT_MAIN) {
        return;
    }
    if (g_battle_script_event_input == 0) {
        g_battle_script_saved_event_input = 0;
    }
    if (g_battle_thread_task_ids[thread][0] == NATIVE_THREAD_TASK_RESUME) {
        g_battle_script_event_input = g_battle_script_unfiltered_controller_input;
    } else {
        g_battle_script_event_input = g_battle_script_saved_event_input;
    }
}
