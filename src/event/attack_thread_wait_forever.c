#include "fft/event_attack.h"

/* Park this event thread while allowing the scheduler to continue. */
void attack_thread_wait_forever(void) {
    while (1) {
        battle_thread_yield();
    }
}
