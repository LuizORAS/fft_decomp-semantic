#include "fft/battle.h"

/* Moves a unit's record to the head of the misc unit list, so list walks reach it
 * first. Returns 1 when moved, 0 when no record has the misc id. */
s32 battle_unit_move_misc_unit_to_head(u32 misc_id) {
    battle_unit_misc_data_t* unit;
    battle_unit_misc_data_t* iterator;
    battle_unit_misc_data_t* head;

    unit = battle_unit_get_misc_data_by_misc_id(misc_id & 0xffff);
    if (unit != 0) {
        /* The list-head word serves as a sentinel record: its previous field
         * (0x000) is the head link. */
        for (iterator = (battle_unit_misc_data_t*)&g_battle_unit_misc_list_head; iterator != 0;
            iterator = iterator->previous) {
            if (iterator->previous == unit) {
                iterator->previous = unit->previous;
                head = (battle_unit_misc_data_t*)&g_battle_unit_misc_list_head;
                unit->previous = head->previous;
                head->previous = unit;
                return 1;
            }
        }
        return 0;
    }
    return 0;
}
