#include "fft/battle.h"

/* Reset every unit's menu record: all 17 bytes cleared, then byte 1 marked unset (0xFF). */
void battle_menu_reset_unit_records(void) {
    s32 i;
    s32 j;

    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        for (j = 16; j >= 0; j--) {
            g_battle_menu_unit_selection_records[i].bytes[j] = 0;
        }
        g_battle_menu_unit_selection_records[i].bytes[1] = 0xFF;
    }
}
