#include "psx/suzuki.h"

/* One of four empty routines between suzuki_smd_init_channel_streams and suzuki_sound_set_sfx_enabled. Nothing
 * on the disc calls it or holds its address. */
void suzuki_noop_800124b4(void) {
}
