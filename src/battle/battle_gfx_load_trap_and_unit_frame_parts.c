#include "fft/battle.h"

/* Build a unit's sprite-part records for one SHP frame, including trap sheets
 * (id >= 0x9b) and Altima's split parts. */
void battle_gfx_load_trap_and_unit_frame_parts(
    battle_unit_misc_data_t* unit, battle_unit_anim_state_t* sprite, u16 frame, u16 animation) {
    s32* frame_table;
    battle_gfx_source_frame_t* frame_data;
    battle_gfx_sprite_display_data_t* display;
    battle_gfx_source_part_t* part;
    u32 y_offset;
    u32 count;
    u16 i;
    u8 flags;
    u16 attr;
    u16 size;
    s32 u;
    u32 tile_index;
    u32 part_flags;
    u32 row;
    u32 mask;

    y_offset = (u16)unit->special_graphic_y_offset;
    /* The selector returns a raw SHP pointer or the -1 sentinel. */
    battle_gfx_select_unit_shp_frame(unit, sprite, frame, animation, &frame_table, (s32*)&frame_data);
    if ((s32)frame_data == -1) {
        main_system_handle_animation_exception(MAIN_SYSTEM_ERROR_SHP_FRAME);
    }
    display = sprite->display;
    count = display->part_count = (frame_data->part_count_and_rotation & 7) + 1;
    display->rotation = g_battle_gfx_sprite_y_rotations[frame_data->part_count_and_rotation >> 3];
    if (count > 8) {
        count = 8;
        main_system_handle_animation_exception(MAIN_SYSTEM_ERROR_FRAME_PART_COUNT);
    }
    flags = frame_data->flags;
    if (unit->spritesheet_id >= BATTLE_SPRITESHEET_ID_CRYSTAL) {
        display->tpage = (flags & 0x60) | 0xb;
        display->clut = unit->vram_palette_id;
        for (i = 0; i < count; i++) {
            part = &frame_data->parts[i];
            attr = part->attributes;
            size = (attr & 0x3c00) >> 10;
            {
                u32 tile;
                s32 part_u;
                s16 part_v;
                s32 width;
                s32 height;
                width = g_battle_gfx_part_sizes[size].width;
                height = g_battle_gfx_part_sizes[size].height;
                tile = attr & 0x3ff;
                part_u = (tile & 0x1f) * 8;
                part_v = y_offset + (tile >> 5) * 8;
                battle_gfx_store_sprite_display_data(display, i, part->x_shift, part->y_shift, part_u, part_v,
                    (s16)(width * 8), (s16)(height * 8), (flags & 0x61) | ((attr & 0xc000) >> 13));
            }
        }
    } else if (frame < (u32)sprite->shp[0]) {
        display->tpage = unit->tpage | (flags & 0x60);
        display->clut = unit->vram_palette_id;
        i = 0;
        if (count != 0) {
            mask = flags & 0x61;
            /* Emits nothing; keeps the target's separate `andi 0xff` of the
             * hoisted mask, which combine would fold into the `andi 0x61`. */
            __asm__("" : "=r"(mask) : "0"(mask));
            do {
                part = &frame_data->parts[i];
                attr = part->attributes;
                size = (attr & 0x3c00) >> 10;
                if (size == 0xe) {
                    if (unit->spritesheet_id == BATTLE_SPRITESHEET_ID_ALTIMA_SECOND_FORM
                        || unit->spritesheet_id == BATTLE_SPRITESHEET_ID_ALTIMA_FIRST_FORM) {
                        u32 tile;
                        s32 part_u;
                        s16 part_v;
                        s32 width;
                        s32 height;
                        tile = attr & 0x3ff;
                        part_u = (tile & 0x1f) * 8;
                        part_v = y_offset + (tile >> 5) * 8;
                        width = g_battle_gfx_part_sizes[size].width;
                        height = g_battle_gfx_part_sizes[size].height;
                        battle_gfx_store_sprite_display_data(display, i, part->x_shift, part->y_shift, part_u, part_v,
                            (s16)(width * 8), (s16)(height * 8), (u8)mask | ((attr & 0xc000) >> 13));
                    } else {
                        tile_index = attr & 0x3ff;
                        u = (tile_index & 0x1f) * 8;
                        row = tile_index >> 5;
                        part_flags = (u8)mask | ((attr & 0xc000) >> 13);
                        battle_gfx_store_sprite_display_data(display, i, part->x_shift, (s16)(part->y_shift + 0x18), u,
                            (row * 8 + 0x18), 0x30, 0x18, part_flags);
                        battle_gfx_store_sprite_display_data(display, i + 1, part->x_shift, part->y_shift, u,
                            (s16)(row * 8), 0x30, 0x18, part_flags | 0x80);
                        display->part_count++;
                        return;
                    }
                } else {
                    u32 tile;
                    s32 part_u;
                    s16 part_v;
                    s32 width;
                    s32 height;
                    width = g_battle_gfx_part_sizes[size].width;
                    height = g_battle_gfx_part_sizes[size].height;
                    tile = attr & 0x3ff;
                    part_u = (tile & 0x1f) * 8;
                    part_v = y_offset + (tile >> 5) * 8;
                    battle_gfx_store_sprite_display_data(display, i, part->x_shift, part->y_shift, part_u, part_v,
                        (s16)(width * 8), (s16)(height * 8), (u8)mask | ((attr & 0xc000) >> 13));
                }
            } while (++i < count);
        }
    } else if (frame < 0xd2) {
        if ((g_battle_gfx_vram_slots[0].owner & 0x1f) == unit->unit_id) {
            battle_gfx_load_unit_frame_parts(unit, display, frame_data, 4);
        } else if ((g_battle_gfx_vram_slots[1].owner & 0x1f) == unit->unit_id) {
            battle_gfx_load_unit_frame_parts(unit, display, frame_data, 5);
        }
    } else if (animation < 600) {
        battle_gfx_load_unit_frame_parts(unit, display, frame_data, 4);
    } else {
        battle_gfx_load_unit_frame_parts(unit, display, frame_data, 5);
    }
}
