#include "fft/main.h"
#include "psx/types.h"

/* Key on the voices queued in each playing music record. The body is main_sound_flush_key_on_inline in
 * fft/main.h, which main_sound_root_counter_2_handler also contains; no retail code calls this copy. */
void main_sound_flush_key_on(void) {
    main_sound_flush_key_on_inline();
}
