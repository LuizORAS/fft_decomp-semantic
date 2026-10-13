#include "fft/battle_unit.h"
#include "psx/types.h"

/* battle_unit_restore_facing_after_attack for every misc id 0..15. */
void battle_unit_restore_all_facings_after_attack(void) {
    s32 i = 0;
    do {
        battle_unit_t* unit = battle_unit_get_misc_data_by_misc_id(i & 0xFFFF);
        if (unit) {
            battle_unit_restore_facing_after_attack(unit);
        }
        i++;
    } while (i < 0x10);
}
