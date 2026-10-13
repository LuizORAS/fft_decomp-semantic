#include "fft/event_require.h"
#include "fft/thread.h"
#include "psx/types.h"

/* Wait until the menu thread ends or a selection result (-1, 0 or 1) is in,
 * then ask the message thread to close and wait for both threads. */
void require_menu_wait_selection_threads(void) {
    while (g_native_threads[REQUIRE_THREAD_SLOT_MENU].is_running != 0) {
        s32 selection = g_require_menu_selection_result;

        if (selection == -1) {
            break;
        }
        if (selection == 1) {
            break;
        }
        if (selection == 0) {
            break;
        }
        thread_yield();
    }
    thread_yield();
    g_native_threads[REQUIRE_THREAD_SLOT_MESSAGE].task_id = NATIVE_THREAD_TASK_STOP_REQUEST;
    battle_thread_set_parameters(REQUIRE_THREAD_SLOT_MESSAGE, 0, 0, 2);
    battle_thread_wait_until_inactive(REQUIRE_THREAD_SLOT_MESSAGE);
    battle_thread_wait_until_inactive(REQUIRE_THREAD_SLOT_MENU);
}
