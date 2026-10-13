#include "fft/thread.h"

void thread_set_four_parameters(s32 slot, s32 first, s32 second, s32 third, s32 fourth) {
    thread_t* thread = &g_threads[slot];
    thread->function_parameter_1 = first;
    thread->function_parameter_2 = second;
    thread->function_parameter_3 = third;
    thread->function_parameter_4 = fourth;
}
