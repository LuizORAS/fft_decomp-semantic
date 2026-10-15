#include "fft/event_equip.h"

/* Park this event thread while allowing the scheduler to continue. */
void equip_thread_wait_forever(void) {
    for (;;) {
        battle_thread_wait_frames(1);
    }
}
