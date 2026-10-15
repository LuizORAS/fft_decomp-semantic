#include "fft/main.h"
#include "psx/types.h"

/* Key off the voices queued in each playing music record and the voices pending release. The body is
 * main_sound_flush_key_off_inline in fft/main.h, which main_sound_root_counter_2_handler also contains; no
 * retail code calls this copy. */
void main_sound_flush_key_off(void) {
    main_sound_flush_key_off_inline();
}
