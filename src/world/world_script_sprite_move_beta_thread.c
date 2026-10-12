#include "fft/world.h"
#include "psx/types.h"

/* Thread body: run the sprite move of parameter 1 in mode 1 (the plain
 * sprite-move thread uses mode 0). */
void world_script_sprite_move_beta_thread(void) {
    world_thread_set_current_task_id(NATIVE_THREAD_TASK_SPRITE_MOVE);
    world_script_run_sprite_move(world_thread_get_current_parameter_1(), 1);
    world_thread_exit_current();
}
