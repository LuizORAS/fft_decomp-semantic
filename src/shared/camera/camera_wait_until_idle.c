#include "fft/battle_camera.h"
#include "fft/thread.h"

/* Yields the thread at least once, then until the camera stops moving. */
void camera_wait_until_idle(void) {
    do {
        thread_yield();
    } while (camera_is_active() != 0);
}
