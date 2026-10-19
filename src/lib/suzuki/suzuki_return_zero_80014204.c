#include "psx/suzuki.h"

/* Return 0, between suzuki_smd_get_instrument_attr and suzuki_sound_sum_resource_words. Nothing on the disc
 * calls it or holds its address. */
int suzuki_return_zero_80014204(void) {
    return 0;
}
