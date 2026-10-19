#include "fft/menu.h"
#include "fft/world.h"
#include "psx/types.h"

/* One row per battle unit, each 17 bytes. A typed row-array version changes the target's
 * induction variables (84 bytes rather than 76), so retain the byte view.
 * Use an integer countdown: decrementing an element pointer after row[0]
 * would form a pointer before the first row. */
enum { WORLD_MENU_UNIT_SELECTION_RECORD_BYTES = (s32)sizeof(battle_menu_unit_selection_record_t) };

/* Reset every unit's menu record: all 17 bytes cleared, then byte 1 marked unset (0xFF). WORLD
 * compiles this text too. */
void menu_reset_unit_records(void) {
    s32 index;
    u8* row;
    s32 count;
    u8 fill;

    fill = 0xFF;
    index = 0;
    row = (u8*)g_world_menu_unit_selection_records;
    do {
        count = WORLD_MENU_UNIT_SELECTION_RECORD_BYTES - 1;
        do {
            row[count] = 0;
            count -= 1;
        } while (count >= 0);
        ((u8*)g_world_menu_unit_selection_records)[index + 1] = fill;
        index += WORLD_MENU_UNIT_SELECTION_RECORD_BYTES;
        row += WORLD_MENU_UNIT_SELECTION_RECORD_BYTES;
    } while (index < BATTLE_UNIT_SLOT_COUNT * WORLD_MENU_UNIT_SELECTION_RECORD_BYTES);
}
