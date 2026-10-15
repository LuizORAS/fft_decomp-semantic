#include "fft/battle.h"
#include "psx/types.h"

/* Thread body of SpriteMove (EVENT_OPCODE_SPRITE_MOVE): run the sprite move
 * (battle_script_run_sprite_move) with the last operand taken as a frame count. */
void battle_script_sprite_move_thread(void) {
    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_SPRITE_MOVE);
    battle_script_run_sprite_move(battle_thread_get_current_parameter_1(), 0);
    battle_thread_exit_current();
}
