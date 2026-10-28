#include "fft/battle_script.h"
#include "fft/thread.h"

/* Thread body of SpriteMove (EVENT_OPCODE_SPRITE_MOVE): run the sprite move
 * (battle_script_run_sprite_move) with the last operand taken as a frame count. */
void script_sprite_move_thread(void) {
    thread_set_current_task_id(THREAD_TASK_SPRITE_MOVE);
    battle_script_run_sprite_move((const event_sprite_move_parameters_t*)battle_thread_get_current_parameter_1(), 0);
    thread_exit_current();
}
