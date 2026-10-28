#include "fft/battle_script.h"
#include "fft/thread.h"

/* Thread body of SpriteMoveBeta (EVENT_OPCODE_SPRITE_MOVE_BETA): run the sprite move with the last
 * operand taken as a speed. */
void script_sprite_move_beta_thread(void) {
    thread_set_current_task_id(THREAD_TASK_SPRITE_MOVE);
    battle_script_run_sprite_move((const event_sprite_move_parameters_t*)battle_thread_get_current_parameter_1(), 1);
    thread_exit_current();
}
