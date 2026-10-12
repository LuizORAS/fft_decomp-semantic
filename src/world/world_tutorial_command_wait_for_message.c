#include "fft/world.h"
#include "psx/types.h"

/* Tutorial command: let the tutorial text continue and move on once its
 * thread has ended. */
void world_tutorial_command_wait_for_message(void) {
    world_text_resume_printing(WORLD_THREAD_SLOT_TUTORIAL_TEXT);
    if (world_thread_is_running(WORLD_THREAD_SLOT_TUTORIAL_TEXT) == 0) {
        g_world_tutorial_command_ptr++;
    }
}
