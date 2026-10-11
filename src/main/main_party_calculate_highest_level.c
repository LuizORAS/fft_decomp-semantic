#include "fft/main.h"

/* Return the highest level in the roster, capped at 99, and store it in
 * g_main_party_highest_level. */
u8 main_party_calculate_highest_level(void) {
    u8 highest;
    s32 i;
    party_data_t* unit;

    highest = 0;
    i = 0;
    do {
        unit = main_party_get_data_pointer(i);
        i += 1;
        if (unit->party_id != PARTY_ID_NONE) {
            if (highest < unit->level) {
                highest = unit->level;
            }
        }
    } while (i < PARTY_ROSTER_SLOT_COUNT);
    if (highest > UNIT_LEVEL_MAX) {
        highest = UNIT_LEVEL_MAX;
    }
    g_main_party_highest_level = highest;
    return highest;
}
