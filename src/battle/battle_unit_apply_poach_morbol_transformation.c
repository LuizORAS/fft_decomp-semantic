#include "fft/battle.h"

/* Carries out a pending Poached or Morbol display flag (status_flags_5_6) and
 * clears it; Poached is tested first and only one is handled per call.
 *
 * Poached starts the unit's red fade-out
 * (battle_gfx_start_unit_red_fade_out). Morbol swaps the sprite
 * for the Morbol spritesheet (VRAM slot, SHP, SEQ, texture page), refreshes the
 * tile palette and the idle animation, then rewrites the battle stats as a Morbol
 * and reinitialises the unit's AI ability data. */
void battle_unit_apply_poach_morbol_transformation(battle_unit_misc_data_t* unit) {
    u32 flags;
    map_tile_t* tile;

    flags = unit->status_flags_5_6;
    if ((flags & (BATTLE_MISC_STATUS_POACHED | BATTLE_MISC_STATUS_MORBOL)) == 0) {
        return;
    }
    if (flags & BATTLE_MISC_STATUS_POACHED) {
        unit->status_flags_5_6 = flags & ~BATTLE_MISC_STATUS_POACHED;
        battle_gfx_start_unit_red_fade_out(unit->unit_id);
        return;
    }
    if ((flags & BATTLE_MISC_STATUS_MORBOL) == 0) {
        return;
    }
    unit->status_flags_5_6 = flags & ~BATTLE_MISC_STATUS_MORBOL;
    unit->special_graphic_y_offset = 0;
    unit->spritesheet_id = BATTLE_SPRITESHEET_ID_MORBOL;
    unit->spritesheet_vram_slot = battle_gfx_claim_spritesheet_slot(BATTLE_SPRITESHEET_ID_MORBOL);
    unit->shp_data = battle_gfx_get_unit_shp_by_spritesheet_id(BATTLE_SPRITESHEET_ID_MORBOL);
    unit->seq_data = battle_gfx_get_unit_seq_by_spritesheet_id(BATTLE_SPRITESHEET_ID_MORBOL);
    unit->stored_palette = 0;
    unit->tpage = unit->spritesheet_vram_slot + BATTLE_GFX_TPAGE_SPRITESHEET_SLOT_0;
    tile = battle_map_get_tile_data_pointer(unit->map_x, unit->map_y, unit->map_z);
    battle_gfx_check_tile_status_palette_mod(unit, tile->flags.bits.shadow_mode, 1);
    battle_unit_store_animation_facing_movement_data(BATTLE_UNIT_ANIMATION_IDLE, unit->facing, unit);
    battle_unit_transform_into_morbol(unit->battle_data);
    battle_ai_init_unit_abilities(unit->battle_data->misc_unit_id);
}
