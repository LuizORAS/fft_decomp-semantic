#include "fft/battle_gfx.h"
#include "fft/battle_map.h"
#include "psx/types.h"

/* Load WEP.SPR: the item palettes (its first 0x200 bytes) into their RAM table, the weapon page at VRAM
 * x 0x240 (BATTLE_GFX_TPAGE_WEAPON), the effect palettes into theirs and the effect page at x 0x200
 * (BATTLE_GFX_TPAGE_EFFECT), the palettes at +0x10400 into CLUT row MAP_CLUT_ROW_WEAPONS through the
 * map palette loader, and a 256x144 block at (0x200, 0x70). */
void battle_gfx_load_wep_spr_data(battle_gfx_weapon_sprite_archive_t* archive) {
    s32 i;
    u8* cursor;

    cursor = (u8*)archive->weapon_clut;
    for (i = 0; i < (s32)sizeof(archive->weapon_clut); i++) {
        ((u8*)g_battle_gfx_item_palettes)[i] = *cursor++;
    }
    LoadTPage((u32*)archive->weapon_pixels, 0, 0, 0x240, 0, 0x100, 0x100);
    for (i = 0, cursor = (u8*)archive->trail_clut; i < (s32)sizeof(archive->trail_clut); i++) {
        ((u8*)g_battle_gfx_eff_palettes)[i] = *cursor++;
    }
    LoadTPage((u32*)archive->trail_pixels, 0, 0, 0x200, 0, 0x100, 0x100);
    battle_map_load_palette_data(archive->particle_clut, MAP_CLUT_ROW_WEAPONS, 0, 1);
    LoadTPage((u32*)archive->particle_pixels, 0, 0, 0x200, 0x70, 0x100, 0x90);
}
