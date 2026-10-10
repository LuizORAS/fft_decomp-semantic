#include "fft/main.h"
#include "psx/types.h"

/* The area-of-effect byte of an ordinary ability's range record; 0 for an id from
 * ABILITY_ID_ITEM_FIRST up, which has no such record. */
u32 main_ability_get_aoe(u32 ability_id) {
    u32 result;

    ability_id &= 0xffff;
    if (ability_id < ABILITY_ID_ITEM_FIRST) {
        result = g_main_ability_aoe_data[ability_id * sizeof(ability_secondary_data_t)];
    } else {
        result = 0;
    }
    return result;
}
