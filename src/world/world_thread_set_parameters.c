#include "fft/thread.h"
#include "fft/world.h"

void world_thread_set_parameters(s32 thread_id, s32 first, s32 second, s32 third) {
    g_native_threads[thread_id].function_parameter_1 = first;
    g_native_threads[thread_id].function_parameter_2 = second;
    g_native_threads[thread_id].function_parameter_3 = third;
}
