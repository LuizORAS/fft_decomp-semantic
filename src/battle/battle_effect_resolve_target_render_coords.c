#include "fft/battle_effect.h"
#include "fft/battle_map.h"
#include "psx/types.h"

/* Resolve one effect target to renderer-space coordinates.
 *
 * A unit target copies the unit's screen coordinate record, and when
 * add_depth is set lowers Y to the tile's own surface height. A tile target
 * projects the tile centre (x * 28 + 14, y * 28 + 14) and takes its height
 * from the tile record, adding the slope depth when add_depth is set. */
void battle_effect_resolve_target_render_coords(
    s32 add_depth, s32 target_index, battle_effect_secondary_init_t* coord_data, SVECTOR* out) {
    map_tile_t* tile;

    if (target_index == BATTLE_EFFECT_TARGET_INDEX_CASTER) {
        if (coord_data->targets[BATTLE_EFFECT_CASTER_ENTRY].target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
            *out = *battle_unit_get_screen_data_ptr_by_misc_id(
                coord_data->targets[BATTLE_EFFECT_CASTER_ENTRY].data.unit.misc_id);
            if (add_depth != 0) {
                s32 height;
                tile = battle_map_get_tile_data_ptr_from_misc_id(
                    coord_data->targets[BATTLE_EFFECT_CASTER_ENTRY].data.unit.misc_id);
                height = -((tile->height + (tile->depth_half_height.value >> MAP_TILE_DEPTH_SHIFT)) * 12);
                if (height < out->vy) {
                    out->vy = height;
                }
            }
        } else {
            tile = battle_map_get_tile_data_pointer(coord_data->targets[BATTLE_EFFECT_CASTER_ENTRY].data.tile.vx,
                coord_data->targets[BATTLE_EFFECT_CASTER_ENTRY].data.tile.vz,
                coord_data->targets[BATTLE_EFFECT_CASTER_ENTRY].data.tile.vy);
            out->vx
                = coord_data->targets[BATTLE_EFFECT_CASTER_ENTRY].data.tile.vx * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
            out->vz
                = coord_data->targets[BATTLE_EFFECT_CASTER_ENTRY].data.tile.vz * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
            if (add_depth != 0) {
                out->vy = -((tile->height + (tile->depth_half_height.value >> MAP_TILE_DEPTH_SHIFT)) * 12);
            } else {
                out->vy = -(tile->height * 12);
            }
        }
    } else {
        if (coord_data->targets[target_index].target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
            *out = *battle_unit_get_screen_data_ptr_by_misc_id(coord_data->targets[target_index].data.unit.misc_id);
            if (add_depth != 0) {
                s32 height;
                tile = battle_map_get_tile_data_ptr_from_misc_id(coord_data->targets[target_index].data.unit.misc_id);
                height = -((tile->height + (tile->depth_half_height.value >> MAP_TILE_DEPTH_SHIFT)) * 12);
                if (height < out->vy) {
                    out->vy = height;
                }
            }
        } else {
            tile = battle_map_get_tile_data_pointer(coord_data->targets[target_index].data.tile.vx,
                coord_data->targets[target_index].data.tile.vz, coord_data->targets[target_index].data.tile.vy);
            out->vx = coord_data->targets[target_index].data.tile.vx * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
            out->vz = coord_data->targets[target_index].data.tile.vz * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
            if (add_depth != 0) {
                out->vy = -((tile->height + (tile->depth_half_height.value >> MAP_TILE_DEPTH_SHIFT)) * 12);
            } else {
                out->vy = -(tile->height * 12);
            }
        }
    }
}
