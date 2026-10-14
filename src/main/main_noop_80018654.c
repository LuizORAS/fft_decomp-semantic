#include "psx/types.h"

/* An empty routine between main_sound_transfer_spu_data and main_sound_wait_for_spu_transfer. Nothing on
 * the disc calls it or holds its address. */
void main_noop_80018654(void) {
}
