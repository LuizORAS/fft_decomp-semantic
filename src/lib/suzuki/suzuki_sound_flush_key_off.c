#include "psx/suzuki.h"

/* Key off the voices queued in each playing music record and the voices pending release. The body is
 * suzuki_sound_flush_key_off_inline in fft/main.h, which suzuki_sound_root_counter_2_handler also contains; no
 * retail code calls this copy. */
void suzuki_sound_flush_key_off(void) {
    suzuki_sound_flush_key_off_inline();
}
