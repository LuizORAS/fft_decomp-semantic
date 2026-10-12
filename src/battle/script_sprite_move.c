#include "fft/battle_event.h"
#include "fft/battle_thread.h"
#include "fft/thread.h"

/* Thread body of SpriteMove: run the sprite move with the last operand taken as a
 * frame count. */
void script_sprite_move(void) {
    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_SPRITE_MOVE);
    script_run_sprite_move((const event_sprite_move_parameters_t*)battle_thread_get_current_parameter_1(), 0);
    battle_thread_exit_current();
}
