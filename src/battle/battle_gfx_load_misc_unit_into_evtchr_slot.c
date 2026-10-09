#include "fft/battle.h"
#include "psx/types.h"

s32 battle_gfx_load_misc_unit_into_evtchr_slot(s32 misc_id, s32 slot) {
    battle_unit_misc_data_t* unit;
    s32 i;
    u8* image_data;
    RECT rect;

    unit = battle_unit_get_misc_data_by_misc_id(misc_id);
    if (g_battle_gfx_vram_slots[slot].owner == 0xfe) {
        for (i = 0; i < 2; i++) {
            if (g_battle_gfx_vram_slots[i].evtchr_load_marker == 0) {
                image_data = g_battle_gfx_vram_slots[i].image_data;
                battle_gfx_decompress_attack_spritesheet(
                    g_battle_gfx_spritesheet_slots[unit->spritesheet_vram_slot].compressed_attack_image, image_data);
                LoadImage(&g_battle_gfx_vram_slots[slot].image_rect, (u32*)image_data);
                rect.x = (unit->spritesheet_vram_slot >> 3) * 64 + 0x340;
                rect.y = (unit->spritesheet_vram_slot & 7) * 32 + 0x100;
                rect.w = 0x40;
                rect.h = 0x20;
                MoveImage(&rect, g_battle_gfx_vram_slots[slot].image_rect.x,
                    g_battle_gfx_vram_slots[slot].image_rect.y + 200);
                g_battle_gfx_vram_slots[slot].owner = misc_id + 0x80;
                g_battle_gfx_vram_slots[slot].evtchr_load_marker = 2;
                g_battle_gfx_vram_slots[slot].evtchr_load_state = 1;
                return 1;
            }
        }
        /* Retail has this exit and the one below as two separate returns. */
        return 0;
    }
    return 0;
}
