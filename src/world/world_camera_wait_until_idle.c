#include "fft/battle.h"
#include "fft/world.h"
#include "psx/types.h"

void world_camera_wait_until_idle(void) {
    do {
        world_thread_yield();
    } while (battle_camera_is_active() != 0);
}
