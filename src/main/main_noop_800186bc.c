#include "psx/types.h"

/* An empty routine between main_sound_wait_for_spu_transfer and SpuInit. Nothing on the disc calls it or
 * holds its address. */
void main_noop_800186bc(void) {
}
