#include "fft/world.h"
#include "psx/types.h"

/* Idle thread body: wait one frame, forever; world_formation_ability_menu_idle_thread is the
 * same loop. */
void world_formation_item_menu_idle_thread(void) {
    for (;;) {
        world_thread_wait_frames(1);
    }
}
