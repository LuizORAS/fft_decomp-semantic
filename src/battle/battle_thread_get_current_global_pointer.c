#include "fft/battle.h"
#include "psx/types.h"

/* Returns the $gp register; battle_thread_start stores it in a new thread's
 * context. */
void* battle_thread_get_current_global_pointer(void) {
    void* gp;
    __asm__ volatile("move %0, $gp" : "=r"(gp));
    return gp;
}
