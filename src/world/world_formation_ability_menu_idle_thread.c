#include "fft/world.h"
#include "psx/types.h"

/* Idle thread body: wait one frame, forever. world_menu_run_ability parks it in the upper menu's
 * child slot. The same routine as world_formation_item_menu_idle_thread, compiled twice. */
void world_formation_ability_menu_idle_thread(void) {
    for (;;) {
        world_thread_wait_frames(1);
    }
}
