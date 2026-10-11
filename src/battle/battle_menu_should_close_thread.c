#include "fft/battle.h"
#include "psx/types.h"

/* Polled by a menu thread each frame; out_flag is the frame's input word.
 *
 * While input is locked or a help window is open the input is cleared and 0
 * returned. If another running thread carries the stop-request task id, the input
 * is cleared too. Returns 1 only when no such thread exists and the current
 * thread's third parameter is set, which is how a menu thread is asked to close. */
s32 battle_menu_should_close_thread(s32* out_flag) {
    s32 i;
    s32 offset;
    s32 task_state;

    if (g_battle_menu_input_lock_mode != 0 || g_battle_menu_help_open != 0) {
        *out_flag = 0;
        return 0;
    }
    i = 0;
    task_state = 3;
    offset = 0;
    for (; i < 16; i++, offset += NATIVE_THREAD_STRIDE) {
        if (i != g_battle_thread_current_id && battle_thread_is_running_asm(i) != 0
            && *(s32*)((s8*)g_battle_thread_task_ids + offset) == task_state) {
            *out_flag = 0;
            break;
        }
    }
    if (i == 16 && battle_thread_get_current_parameter_3() != 0) {
        return 1;
    }
    return 0;
}
