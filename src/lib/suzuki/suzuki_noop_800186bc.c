#include "psx/suzuki.h"

/* An empty routine between suzuki_sound_wait_for_spu_transfer and SpuInit. Nothing on the disc calls it or
 * holds its address. */
void suzuki_noop_800186bc(void) {
}
