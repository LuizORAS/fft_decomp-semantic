#include "fft/battle.h"

/* Remove every record from the misc unit list (battle_unit_remove_misc): when a map jumps out and when
 * the battle loop ends. */
void battle_unit_clear_misc_units(void) {
    battle_unit_misc_data_t* unit;

    unit = g_battle_unit_misc_list_head;
    while (unit != 0) {
        battle_unit_remove_misc(unit);
        unit = unit->previous;
    }
}
