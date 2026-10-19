#include "fft/thread.h"
#include "fft/world.h"

/* Thread body: run the sprite move of parameter 1 in mode 1 (the plain
 * sprite-move thread uses mode 0). */
void world_script_sprite_move_beta_thread(void) {
    thread_set_current_task_id(THREAD_TASK_SPRITE_MOVE);
    world_script_run_sprite_move((const event_sprite_move_parameters_t*)world_thread_get_current_parameter_1(), 1);
    thread_exit_current();
}
