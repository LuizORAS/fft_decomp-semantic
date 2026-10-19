#include "fft/unit.h"
#include "fft/world.h"
#include "psx/types.h"

/* 21 rows of 17 bytes; the same table as world_menu_reset_unit_records. */
enum { WORLD_MENU_UNIT_SELECTION_RECORD_BYTES = (s32)sizeof(battle_menu_unit_selection_record_t) };

/* WORLD twin of battle_menu_reset_unit_record. */
void world_menu_reset_unit_record(s32 id) {
    u8* table;
    battle_stats_t* stats;
    u8* row;
    s32 index;
    s32 i;
    u8 fill;

    table = (u8*)g_world_menu_unit_selection_records;
    stats = battle_unit_get_stats_from_battle_id(id);
    fill = 0xFF;
    row = table;
    index = id * WORLD_MENU_UNIT_SELECTION_RECORD_BYTES;
    row += index;
    for (i = 0; i < WORLD_MENU_UNIT_SELECTION_RECORD_BYTES; i++) {
        if (stats->auto_battle_setting == BATTLE_AUTO_BATTLE_MANUAL) {
            row[i] = 0;
            ((u8*)g_world_menu_unit_selection_records)[index + 1] = fill;
        }
    }
}
