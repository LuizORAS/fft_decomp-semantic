#include "fft/menu.h"
#include "fft/world.h"
#include "psx/types.h"

/* Reset every unit's menu record: all 17 bytes cleared, then byte 1 marked unset (0xFF). WORLD
 * compiles this text too. One row per battle unit; a typed row-array version changes the
 * target's induction variables (84 bytes rather than 76), so retain the byte view. Use an
 * integer countdown: decrementing an element pointer after row[0] would form a pointer before
 * the first row. */
void menu_reset_unit_records(void) {
    s32 index;
    u8* row;
    s32 count;
    u8 fill;

    fill = 0xFF;
    index = 0;
    row = (u8*)g_world_menu_unit_selection_records;
    do {
        count = MENU_UNIT_SELECTION_RECORD_BYTES - 1;
        do {
            row[count] = 0;
            count -= 1;
        } while (count >= 0);
        ((u8*)g_world_menu_unit_selection_records)[index + 1] = fill;
        index += MENU_UNIT_SELECTION_RECORD_BYTES;
        row += MENU_UNIT_SELECTION_RECORD_BYTES;
    } while (index < BATTLE_UNIT_SLOT_COUNT * MENU_UNIT_SELECTION_RECORD_BYTES);
}
