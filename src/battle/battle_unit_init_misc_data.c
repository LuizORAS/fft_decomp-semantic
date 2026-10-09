#include "fft/battle.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Claims and initialises Misc Unit Data for one queued unit: spritesheet VRAM
 * slot and palette, map position, displays and the three weapon/body-part
 * sprite records, then links it at the head of the misc-unit list.
 *
 * `id` is copied before the claim call, which keeps the unextended copy in
 * fp. The unit block pointer is assigned after the call to
 * battle_unit_set_move_and_screen_coords: because it lives across calls,
 * sched1 still hoists it to the join, just after that call's argument copy.
 * Reusing `sprite` for the loop blocks keeps both in s3, so it first views
 * the unit's own anim state at 0x1d8 through the prefix the two block types
 * share.
 * The contradictory weapon range test (<0x86, <0x3c, >=0x4a, !=0x5d) is
 * literally what the target branches encode (the call is dead code). */
battle_unit_misc_data_t* battle_unit_init_misc_data(s32 map_x, s32 map_y, s32 map_level, s16 facing, s32 spritesheet_id,
    s16 palette, s16 misc_id, battle_stats_t* stats, u32 flags, u8* data) {
    battle_unit_misc_data_t* unit;
    battle_unit_sprite_block_t* sprite;
    map_tile_t* tile;
    s32 unit_id;
    s32 slot;
    u16 id;
    s32 i;
    battle_unit_misc_data_t** head;
    u8 tile_x;
    u8 tile_y;
    u8 tile_z;

    id = spritesheet_id;
    unit = battle_unit_claim_misc_slot(misc_id);
    if (unit == 0) {
        return 0;
    }
    unit_id = unit->unit_id;
    unit->spritesheet_id = id;
    slot = unit->spritesheet_vram_slot = battle_gfx_claim_spritesheet_slot(spritesheet_id);
    if (slot == 0xffff) {
        g_battle_unit_misc_slot_flags[unit->unit_id].in_use = 0;
        return 0;
    }
    if (id >= 0x60 && id < 0x86 && stats != 0) {
        palette = stats->sprite_palette;
    }
    if (palette >= 6) {
        palette = 0;
    }
    unit->stored_palette = palette;
    unit->statuses_to_remove_5_6 = 0;
    unit->statuses_to_remove_1_4 = 0;
    unit->statuses_to_add_5_6 = 0;
    unit->statuses_to_add_1_4 = 0;
    unit->status_flags_5_6 = 0;
    unit->status_flags_1_4 = 0;
    battle_gfx_load_spritesheet_into_vram_slot(data, slot, unit_id, unit, palette);
    tile = battle_map_get_tile_data_pointer(map_x, map_y, map_level);
    battle_gfx_check_tile_status_palette_mod(unit, tile->flags.bits.shadow_mode, 1);
    battle_map_load_palette_data(g_battle_gfx_shadow_palette, MAP_PALETTE_BANK_UNIT_SHADOWS, unit_id, 0);
    unit->vram_palette_id = GetClut(unit_id << 4, 0x1e3);
    unit->tpage = slot + 0x14;
    if (flags == 0) {
        unit->draw_enabled = 1;
        unit->sprite_graphic_trigger = 1;
    } else {
        unit->draw_enabled = 0;
        unit->sprite_graphic_trigger = 0;
    }
    unit->standing_on_unit_id_plus_one = 0;
    unit->battle_data = stats;
    unit->requested_animation = 0;
    unit->facing = facing * 1024;
    unit->camera_facing_sixteenth.s = unit->camera_facing_quadrant.s = unit->attack_facing = -1;
    unit->mount_state = BATTLE_MISC_MOUNT_STATE_NONE;
    unit->horizontal_flip_flag = 0;
    unit->depth_height_offset = 0;
    unit->float_bob_offset = 0;
    unit->float_bob_phase = 0;
    unit->step_phase = BATTLE_MOVE_STEP_PHASE_NONE;
    unit->movement_path_count = 0;
    unit->sprite_display_flags.half = 0;
    unit->layer_priority = 0;
    unit->turn_action_flags = 0;
    unit->special_graphic_y_offset = 0;
    unit->distortion_animation_id = 0;
    unit->distortion_phase = 0;
    tile_x = map_x;
    unit->map_x = tile_x;
    unit->movement.word &= ~BATTLE_MOTION_FLAG_SUPPRESS_FACING_UPDATE;
    tile_y = map_y;
    unit->map_y = tile_y;
    unit->movement.word &= ~BATTLE_MOTION_FLAG_SUPPRESS_PALETTE_UPDATE;
    tile_z = map_level;
    unit->map_z = tile_z;
    unit->movement.word &= ~BATTLE_MOTION_FLAG_ALTERNATE_SHORT_HOP;
    unit->movement.word &= ~BATTLE_MOTION_FLAG_FLOAT;
    unit->movement.word &= ~BATTLE_MOTION_FLAG_FORCE_ANIMATION_UPDATE;
    unit->movement.bytes.destination_x = tile_x;
    unit->movement.bytes.destination_y = tile_y;
    unit->movement.bytes.destination_z = tile_z;
    battle_unit_set_move_and_screen_coords(unit);
    sprite = (battle_unit_sprite_block_t*)&unit->sprite_graphic_trigger;
    unit->velocity.vz = 0;
    unit->velocity.vy = 0;
    unit->velocity.vx = 0;
    unit->walk_speed.word = 0x2000;
    unit->seq_offset.vz = 0;
    unit->seq_offset.vy = 0;
    unit->seq_offset.vx = 0;
    unit->screen_offset.vz = 0;
    unit->screen_offset.vy = 0;
    unit->screen_offset.vx = 0;
    unit->event_offset.vz = 0;
    unit->event_offset.vy = 0;
    unit->event_offset.vx = 0;
    unit->fade_out_phase = BATTLE_MISC_FADE_OUT_NONE;
    unit->shadow_graphic_trigger = 1;
    unit->shadow_dirty = 1;
    unit->shadow_counter = 0;
    unit->numeric_display_active = 0;
    unit->numeric_display_selector = 0;
    unit->numeric_display_progress = 0;
    unit->numeric_displays[0] = battle_gfx_init_numeric_sprite_display(unit_id, 0);
    unit->numeric_displays[1] = battle_gfx_init_numeric_sprite_display(unit_id, 1);
    unit->numeric_displays[2] = battle_gfx_init_numeric_sprite_display(unit_id, 2);
    unit->numeric_displays[0]->scale_x = unit->numeric_displays[0]->scale_y = unit->numeric_displays[1]->scale_x
        = unit->numeric_displays[1]->scale_y = unit->numeric_displays[2]->scale_x = unit->numeric_displays[2]->scale_y
        = ONE;
    unit->numeric_displays[0]->tpage = unit->numeric_displays[1]->tpage = unit->numeric_displays[2]->tpage
        = BATTLE_GFX_TPAGE_FRAME;
    unit->numeric_displays[0]->clut = unit->numeric_displays[1]->clut = unit->numeric_displays[2]->clut
        = unit->vram_palette_id + 0x100;
    unit->numeric_displays[0]->part_count = unit->numeric_displays[1]->part_count
        = unit->numeric_displays[2]->part_count = 1;
    unit->status_bubble_active = 0;
    unit->status_bubble_id = 0;
    unit->status_bubble_timer = 0;
    unit->status_bubble_display = battle_gfx_init_status_bubble_sprite_display(unit_id);
    unit->item_display_active = 0;
    unit->item_display = battle_gfx_init_item_sprite_display(unit_id);
    unit->position_copies_active = 0;
    unit->sprite_display_section = (battle_gfx_sprite_display_data_t*)battle_gfx_init_unit_sprite_display(unit_id);
    sprite->graphic_type = BATTLE_SPRITE_GRAPHIC_UNIT;
    unit->shp_data = unit->saved_shp_data = battle_gfx_get_unit_shp_by_spritesheet_id(id);
    unit->seq_data = unit->saved_seq_data = battle_gfx_get_unit_seq_by_spritesheet_id(id);
    sprite->wait = 1;
    sprite->restart_count = 0;
    sprite->script_pos = 0;
    sprite->animation_id = 6;
    sprite->frame = 0;
    sprite->flags = 0;
    sprite->wait_bias = 0;
    sprite->frame_bias = 0;
    sprite->loop_count = 0;
    unit->sprite_display_section->scale_x = ONE;
    unit->sprite_display_section->scale_y = ONE;
    unit->sprite_display_section->rotation = 0;
    battle_unit_set_animation_based_on_status(unit);
    battle_gfx_run_unit_seq_script(unit, (battle_unit_anim_state_t*)sprite, 1);
    for (i = 0; i < 3; i++) {
        sprite = &unit->sprite_blocks[i];
        sprite->display = battle_gfx_init_weapon_sprite_display(unit_id, i);
        if (i == 0) {
            sprite->graphic_type = BATTLE_SPRITE_GRAPHIC_WEAPON;
            sprite->shp = (battle_gfx_weapon_shp_t*)battle_gfx_get_weapon_shp_by_spritesheet_id(id);
            sprite->seq = (u8**)battle_gfx_get_weapon_seq_by_spritesheet_id(id);
        } else {
            sprite->graphic_type = BATTLE_SPRITE_GRAPHIC_EFFECT;
            sprite->shp = (battle_gfx_weapon_shp_t*)battle_gfx_get_effect_shp_data(id);
            sprite->seq = (u8**)battle_gfx_get_effect_seq_data(id);
        }
        sprite->trigger = 0;
        sprite->wait = 1;
        sprite->script_pos = 0;
        sprite->animation_id = 0;
        sprite->frame = 0;
        sprite->flags = 0;
        sprite->wait_bias = 0;
        sprite->frame_bias = 0;
        sprite->loop_count = 0;
        sprite->display->scale_x = ONE;
        sprite->display->scale_y = ONE;
        sprite->display->rotation = 0;
        if ((s16)id < 0x86 && (s16)id < 0x3c && (s16)id >= 0x4a && (s16)id != 0x5d) {
            battle_gfx_run_wep_eff_seq_script(unit, &unit->sprite_blocks[i], i, 1);
        }
    }
    head = &g_battle_unit_misc_list_head;
    unit->previous = *head;
    *head = unit;
    battle_gfx_set_thrown_item_graphic_palette(
        unit->battle_data->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_WEAPON], unit);
    battle_status_init_special_flag_enabling(unit->battle_data->misc_unit_id);
    unit->movement.word |= BATTLE_MOTION_FLAG_SUPPRESS_SFX_AND_LANDING_EFFECTS;
    battle_unit_update_display_by_misc_id(unit->unit_id);
    battle_unit_set_animation_based_on_status(unit);
    unit->movement.word &= ~BATTLE_MOTION_FLAG_SUPPRESS_SFX_AND_LANDING_EFFECTS;
    return unit;
}
