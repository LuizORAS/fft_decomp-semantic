#include "fft/event_option.h"
#include "fft/thread.h"

/* Yield every frame forever: the stand-in thread that option_menu_start_crystal_pickup_threads keeps in
 * THREAD_BORROWED_SLOT_AT_LIST_MENU_CHILD while the thread below the ability list runs. */
void option_thread_wait_forever(void) {
    for (;;) {
        thread_yield();
    }
}
