#include "psx/suzuki.h"

/* One of two empty routines between suzuki_smd_unload_music and suzuki_smd_reset_music. Nothing on the disc calls it
 * or holds its address. */
void suzuki_noop_800121d4(void) {
}
