#include "psx/suzuki.h"

/* Key on the voices queued in each playing music record. The body is suzuki_sound_flush_key_on_inline in
 * fft/main.h, which suzuki_sound_root_counter_2_handler also contains; no retail code calls this copy. */
void suzuki_sound_flush_key_on(void) {
    suzuki_sound_flush_key_on_inline();
}
