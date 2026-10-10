#include "fft/main.h"

/* The range byte of an ordinary ability's range record; 0 for an id from
 * ABILITY_ID_ITEM_FIRST up, which has no such record. */
u32 main_ability_get_range(u32 ability_id) {
    u32 range;

    ability_id &= 0xffff;
    if (ability_id < ABILITY_ID_ITEM_FIRST) {
        range = g_main_ability_range_data[ability_id].range;
    } else {
        range = 0;
    }
    return range;
}
