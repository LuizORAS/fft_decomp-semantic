#include "fft/event_option.h"

/* Yield every frame forever: the stand-in thread that option_menu_start_dead_unit_threads keeps in
 * NATIVE_BORROWED_THREAD_SLOT_AT_LIST_MENU_CHILD while the thread below the ability list runs. */
void option_thread_wait_forever(void) {
    for (;;) {
        battle_thread_yield();
    }
}
