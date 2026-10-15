#include "fft/battle.h"
#include "psx/types.h"
/* The number of records in the misc unit list. No retail code calls it. */
s32 battle_unit_count_from_misc_data(void) {
    s32 count = 0;
    battle_unit_misc_data_t* unit = g_battle_unit_misc_list_head;
    if (unit != 0) {
        do {
            unit = unit->previous;
            count++;
        } while (unit != 0);
    }
    return count;
}
