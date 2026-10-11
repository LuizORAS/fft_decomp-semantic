#include "psx/suzuki.h"
#include "psx/types.h"

/* Commit pending key-offs; shared with the root-counter handler. */
void suzuki_sound_flush_key_off(void) {
    suzuki_sound_flush_key_off_inline();
}
