#include "fft/thread.h"

/* Return $gp, which thread_start stores in each new thread. */
void* thread_get_current_global_pointer(void) {
    void* global_pointer;

    __asm__("move %0, $gp" : "=r"(global_pointer));
    return global_pointer;
}
