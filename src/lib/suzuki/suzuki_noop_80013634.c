#include "psx/suzuki.h"

/* An empty routine between suzuki_smd_get_active_voice_mask and suzuki_sound_init_sfx_music. Nothing on the disc calls
 * it or holds its address. */
void suzuki_noop_80013634(void) {
}
