#include "fft/thread.h"
#include "fft/world.h"

/* Thread body of SpriteMove (task THREAD_TASK_SPRITE_MOVE): run the sprite move
 * (world_script_run_sprite_move) with the last operand taken as a frame count. */
void world_script_sprite_move_thread(void) {
    thread_set_current_task_id(THREAD_TASK_SPRITE_MOVE);
    world_script_run_sprite_move((const event_sprite_move_parameters_t*)world_thread_get_current_parameter_1(), 0);
    thread_exit_current();
}
