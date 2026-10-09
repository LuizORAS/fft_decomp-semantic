#include "fft/battle.h"

u8* battle_gfx_get_unit_seq_by_spritesheet_id(u32 index) {
    s32 value;

    value = g_battle_gfx_spritesheet_data[index & 0xffff].seq_id;
    if (value < 5) {
        return g_battle_gfx_unit_seq_tables + value * 0x408;
    }
    return g_battle_gfx_boss_seq_tables;
}
