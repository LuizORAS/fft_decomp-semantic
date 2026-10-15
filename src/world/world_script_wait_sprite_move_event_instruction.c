#include "fft/world.h"
#include "psx/types.h"

/* WORLD's copy of battle_script_wait_sprite_move_event_instruction (WaitSpriteMove): yield until no
 * thread runs the sprite-move task (NATIVE_THREAD_TASK_SPRITE_MOVE) for the unit's sprite; an absent
 * unit (EVENT_MISC_ID_NONE) has nothing to wait for. WORLD scans thread slots 0-16. */
void world_script_wait_sprite_move_event_instruction(s32 unit_id) {
    s32 misc_id;
    s32 i;

    misc_id = world_script_get_misc_id(unit_id);
    if (misc_id == EVENT_MISC_ID_NONE) {
        return;
    }
    for (;;) {
        for (i = 0; i < 17; i++) {
            if (world_thread_is_running_asm(i) != 0
                && g_world_thread_contexts[i].task_id == NATIVE_THREAD_TASK_SPRITE_MOVE
                && g_world_thread_contexts[i].task_words[0] == misc_id) {
                break;
            }
        }
        if (i == 17) {
            return;
        }
        world_thread_yield();
    }
}
