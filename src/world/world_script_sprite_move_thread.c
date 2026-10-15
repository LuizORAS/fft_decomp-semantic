#include "fft/world.h"
#include "psx/types.h"

/* Thread body of SpriteMove (task NATIVE_THREAD_TASK_SPRITE_MOVE): run the sprite move
 * (world_script_run_sprite_move) with the last operand taken as a frame count. */
void world_script_sprite_move_thread(void) {
    world_thread_set_current_task_id(NATIVE_THREAD_TASK_SPRITE_MOVE);
    world_script_run_sprite_move(world_thread_get_current_parameter_1(), 0);
    world_thread_exit_current();
}
