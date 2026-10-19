#include "fft/main.h"

/* Return a default ability's range from its range data, or 0 for an id of 0x170 or more.
 * Referenced nowhere on the disc. */
u32 main_ability_get_range(u32 ability_id) {
    u16 index;

    index = ability_id;
    return index < ABILITY_ID_ITEM_FIRST ? g_main_ability_range_data[index].range : 0;
}
