#include "fft/battle.h"
#include "fft/battle_camera.h"

/* Yields the thread at least once, then until the camera stops moving. */
void battle_camera_wait_until_idle(void) {
    do {
        battle_thread_yield();
    } while (battle_camera_is_active() != 0);
}
