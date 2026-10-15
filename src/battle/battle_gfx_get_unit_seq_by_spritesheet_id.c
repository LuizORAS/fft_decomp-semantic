#include "fft/battle.h"

/* The SEQ table of a sheet: one of the five resident ones, or the table of
 * the battle's RUKA, ARUTE or KANZEN file. */
u8* battle_gfx_get_unit_seq_by_spritesheet_id(u32 spritesheet_id) {
    s32 value;

    value = g_battle_gfx_spritesheet_data[spritesheet_id & 0xffff].seq_id;
    if (value < BATTLE_SEQ_RUKA) {
        return g_battle_gfx_unit_seq_tables + value * 0x408;
    }
    return g_battle_gfx_boss_seq_tables;
}
