#include "fft/world.h"
#include "psx/types.h"

/* Same as world_formation_item_menu_idle_thread, at its own address. */
void world_formation_ability_menu_idle_thread(void) {
    for (;;) {
        world_thread_wait_frames(1);
    }
}
