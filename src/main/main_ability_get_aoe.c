#include "fft/main.h"
#include "psx/types.h"

/* The area-of-effect byte of an ordinary ability's range record; 0 for an id from
 * ABILITY_ID_ITEM_FIRST up, which has no such record. */
u32 main_ability_get_aoe(u32 ability_id) {
    u16 index;

    index = ability_id;
    return index < ABILITY_ID_ITEM_FIRST ? g_main_ability_range_data[index].aoe : 0;
}
