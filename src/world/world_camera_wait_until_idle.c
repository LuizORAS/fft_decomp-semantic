#include "fft/battle_camera.h"
#include "fft/world.h"

void world_camera_wait_until_idle(void) {
    do {
        world_thread_yield();
    } while (camera_is_active() != 0);
}
