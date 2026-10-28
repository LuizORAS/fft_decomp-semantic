#include "fft/thread.h"
#include "fft/world.h"

/* Same as world_formation_item_menu_idle_thread, at its own address. */
void world_formation_ability_menu_idle_thread(void) {
    for (;;) {
        thread_wait_frames(1);
    }
}
