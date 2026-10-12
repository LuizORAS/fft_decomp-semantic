#include "fft/battle_menu.h"
#include "psx/stddef.h"
#include "psx/types.h"

/* Clear whole records through a byte cursor: indexed record fields change the
 * native loop induction. Byte 1 remains unset (0xff). */
void battle_menu_reset_unit_records(void) {
    s32 index;
    u8* row;
    s32 count;
    u8 fill;

    fill = 0xFF;
    index = 0;
    row = (u8*)g_battle_menu_unit_selection_records;
    do {
        count = sizeof(menu_unit_selection_record_t) - 1;
        do {
            row[count] = 0;
            count -= 1;
        } while (count >= 0);
        ((u8*)g_battle_menu_unit_selection_records)[index + offsetof(menu_unit_selection_record_t, _unknown_01)] = fill;
        index += sizeof(menu_unit_selection_record_t);
        row += sizeof(menu_unit_selection_record_t);
    } while (index < (s32)sizeof(g_battle_menu_unit_selection_records));
}
