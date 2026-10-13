#include "fft/thread.h"
#include "fft/world.h"

/* Menu thread for a refused command: queue the invalid sound and exit. */
void world_menu_stop_thread_with_invalid_sound(void) {
    world_sound_queue_sfx_invalid();
    thread_exit_current();
}
