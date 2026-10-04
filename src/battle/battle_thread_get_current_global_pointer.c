#include "psx/types.h"

/* Return $gp, which battle_thread_start stores in each new thread. */
void* battle_thread_get_current_global_pointer(void) {
    void* gp;
    __asm__ volatile("move %0, $gp" : "=r"(gp));
    return gp;
}
