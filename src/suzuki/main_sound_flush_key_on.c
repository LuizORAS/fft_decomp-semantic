#include "psx/suzuki.h"
#include "psx/types.h"

/* Commit pending key-ons; shared with the root-counter handler. */
void main_sound_flush_key_on(void) {
    main_sound_flush_key_on_inline();
}
