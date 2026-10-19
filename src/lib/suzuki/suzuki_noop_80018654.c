#include "psx/suzuki.h"

/* An empty routine between suzuki_sound_transfer_spu_data and suzuki_sound_wait_for_spu_transfer. Nothing on
 * the disc calls it or holds its address. */
void suzuki_noop_80018654(void) {
}
