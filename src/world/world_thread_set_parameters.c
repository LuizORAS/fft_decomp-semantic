#include "fft/world.h"

/* Store parameters 1-3 in thread_id's slot, which the thread reads back with
 * world_thread_get_current_parameter_1-3. */
void world_thread_set_parameters(s32 thread_id, s32 first, s32 second, s32 third) {
    g_world_threads[thread_id].function_parameter_1 = first;
    g_world_threads[thread_id].function_parameter_2 = second;
    g_world_threads[thread_id].function_parameter_3 = third;
}
