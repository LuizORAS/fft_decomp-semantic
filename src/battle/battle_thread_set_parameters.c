#include "fft/battle.h"
#include "psx/types.h"

/* Store parameters 1-3 in thread_id's slot, which the thread reads back with
 * battle_thread_get_current_parameter_1-3. */
void battle_thread_set_parameters(s32 thread_id, s32 parameter_1, s32 parameter_2, s32 parameter_3) {
    native_thread_t* thread = &g_battle_threads[thread_id];
    thread->function_parameter_1 = parameter_1;
    thread->function_parameter_2 = parameter_2;
    thread->function_parameter_3 = parameter_3;
}
