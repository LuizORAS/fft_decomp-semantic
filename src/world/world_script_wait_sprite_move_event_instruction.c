#include "fft/script.h"
#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* WORLD's copy of script_wait_sprite_move_event_instruction (WaitSpriteMove): yield until no
 * thread runs the sprite-move task (THREAD_TASK_SPRITE_MOVE) for the unit's sprite; an absent
 * unit (EVENT_RUNTIME_ID_NONE) has nothing to wait for. WORLD scans thread slots 0-16. */
void world_script_wait_sprite_move_event_instruction(s32 unit_id) {
    s32 runtime_id;
    s32 i;

    runtime_id = script_resolve_runtime_id(unit_id);
    if (runtime_id == EVENT_RUNTIME_ID_NONE) {
        return;
    }
    for (;;) {
        for (i = 0; i < 17; i++) {
            if (world_thread_is_running_asm(i) != 0 && g_thread_contexts[i].task_id == THREAD_TASK_SPRITE_MOVE
                && g_thread_contexts[i].task_words[0] == runtime_id) {
                break;
            }
        }
        if (i == 17) {
            return;
        }
        thread_yield();
    }
}
