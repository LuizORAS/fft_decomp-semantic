#include "fft/battle.h"

/* Make item `graphic_id` the one a unit's weapon and effect blocks draw.
 *
 * The item's two palettes of g_battle_gfx_item_graphic_data go to the unit's
 * CLUTs of the ITEM_A and ITEM_B rows, and its type selects the frame set of
 * the weapon SHP; a thrown weapon, any Throw ability and Throw Stone use
 * set 0x15. get_item_data_pointer takes the item id still in $a0. */
void battle_gfx_set_thrown_item_graphic_palette(s32 graphic_id, battle_unit_misc_data_t* unit) {
    u8* item;
    u32 weapon_type;
    s32 clut_index;
    s32 idx;
    battle_unit_misc_data_t* unit2;
    /* The last test re-reads the ability id rather than reusing the value the
     * range test loaded; assigning it through its own local stops cse folding
     * the two reads into one. */
    u16 ability_id;

    unit2 = unit;
    item = get_item_data_pointer();
    weapon_type = item[5];
    if ((weapon_type >= ITEM_TYPE_SHURIKEN)
        || ((u32)(unit2->used_ability_id - ABILITY_ID_THROW_FIRST) < ABILITY_ID_JUMP_FIRST - ABILITY_ID_THROW_FIRST)
        || ((ability_id = unit->used_ability_id), ability_id == ABILITY_ID_BASIC_SKILL_THROW_STONE)) {
        weapon_type = 0x15;
    }
    idx = graphic_id * 2;
    clut_index = unit->unit_id;
    /* The header types this as u8[0x200]; the palettes are 16 colours of
     * two bytes, so the index has to stay in halfwords or the stride halves. */
    battle_map_load_palette_data(&((u16*)g_battle_gfx_item_palettes)[(g_battle_gfx_item_graphic_data[idx] >> 4) * 16],
        MAP_CLUT_ROW_ITEM_A, clut_index, 0);
    battle_map_load_palette_data(&((u16*)g_battle_gfx_item_palettes)[(g_battle_gfx_item_graphic_data[idx] & 0xF) * 16],
        MAP_CLUT_ROW_ITEM_B, clut_index, 0);
    unit->used_item_or_weapon_id = graphic_id;
    unit->equipped_weapon_type = weapon_type;
}
