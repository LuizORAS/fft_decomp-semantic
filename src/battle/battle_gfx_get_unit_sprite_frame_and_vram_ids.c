#include "fft/battle.h"
#include "psx/types.h"

/*
 * Give a unit's CLUT, texture page, graphic height and v offset, and return
 * the SHP frame it is showing: the submerged one for a rider. The crystal
 * and later status sheets share the OTHER texture page.
 */
u8* battle_gfx_get_unit_sprite_frame_and_vram_ids(u32 misc_id, battle_unit_sprite_query_t* out) {
    battle_unit_misc_data_t* unit;

    unit = battle_unit_get_misc_data_by_misc_id(misc_id & 0xffff);
    if (unit == 0) {
        return 0;
    }

    out->clut = unit->clut;
    if (unit->spritesheet_id >= BATTLE_SPRITESHEET_ID_CRYSTAL) {
        out->tpage = BATTLE_GFX_TPAGE_OTHER;
    } else {
        out->tpage = unit->tpage;
    }
    out->graphic_height = g_battle_gfx_spritesheet_data[unit->spritesheet_id].graphic_height;
    out->graphic_y_offset = unit->special_graphic_y_offset;

    if (unit->mount_state != BATTLE_MISC_MOUNT_STATE_RIDER) {
        return unit->shp_data->frames[unit->animation_frame];
    } else {
        return unit->shp_data->submerged_frames[unit->animation_frame];
    }
}
