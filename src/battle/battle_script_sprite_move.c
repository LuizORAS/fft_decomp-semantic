#include "fft/battle.h"
#include "psx/types.h"

/* Thread body of SpriteMove: run the sprite move with the last operand taken as a
 * frame count. */
void battle_script_sprite_move(void) {
    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_SPRITE_MOVE);
    battle_script_run_sprite_move((const event_sprite_move_parameters_t*)battle_thread_get_current_parameter_1(), 0);
    battle_thread_exit_current();
}
