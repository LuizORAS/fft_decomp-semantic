#include "fft/main.h"
#include "psx/types.h"

/* Return a default ability's area of effect from its range data, or 0 for an id of 0x170 or
 * more. Referenced nowhere on the disc. */
u32 main_ability_get_aoe(u32 ability_id) {
    u16 index;

    index = ability_id;
    return index < ABILITY_ID_ITEM_FIRST ? g_main_ability_range_data[index].aoe : 0;
}
