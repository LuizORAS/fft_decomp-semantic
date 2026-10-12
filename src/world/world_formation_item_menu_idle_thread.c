#include "fft/world.h"

/* Idle thread body: wait one frame, forever. world_formation_item_menu parks it in the upper menu's
 * child slot. world_formation_ability_menu_idle_thread is the same routine compiled twice. */
void world_formation_item_menu_idle_thread(void) {
    for (;;) {
        world_thread_wait_frames(1);
    }
}
