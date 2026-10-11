#include "psx/suzuki.h"
#include "psx/types.h"

/* Commit pending key-offs; shared with the root-counter handler. */
void main_sound_flush_key_off(void) {
    main_sound_flush_key_off_inline();
}
