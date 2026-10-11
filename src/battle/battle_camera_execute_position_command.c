#include "fft/battle.h"
#include "psx/types.h"

/* Timeline track 2 handler: sets the effect camera position target and duration.
 *
 * Mode bits 0x1e0 pick the source: effect target `target` (0x0, which also
 * saves the current camera position to g_battle_effect_camera_position_start), the given vector (0x40),
 * the map centre (0x80), the current camera (0xc0), the saved position at
 * g_battle_effect_camera_position_saved (0x100), the caster entry (0x140), the average of all unit
 * targets (0x180) or the tile at g_battle_effect_target_tile (0x1c0). All but 0x40 add offset
 * when present. Bits 0x1e00 are kept in g_battle_effect_camera_position_mode. */
void battle_camera_execute_position_command(s32 command, s32 target, s32 duration, VECTOR* offset) {
    VECTOR sum;
    VECTOR* coords;
    SVECTOR* screen;
    s32 i;

    switch (command & BATTLE_EFFECT_CAMERA_SOURCE_MASK) {
    case BATTLE_EFFECT_CAMERA_SOURCE_TARGET:
        if (g_battle_effect_context.targets[target].target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
            battle_effect_copy_misc_unit_screen_location(
                g_battle_effect_context.targets[target].data.unit.misc_id, &g_battle_effect_camera_position_target);
        } else {
            battle_effect_convert_tile_coords_to_world_coords(
                &g_battle_effect_context.targets[target].data.tile, &g_battle_effect_camera_position_target);
        }
        coords = battle_camera_get_current_real_coords();
        battle_effect_shift_vector_right_12(coords, &g_battle_effect_camera_position_start);
        if (offset) {
            battle_effect_add_vectors(
                offset, &g_battle_effect_camera_position_target, &g_battle_effect_camera_position_target);
        }
        break;
    case BATTLE_EFFECT_CAMERA_SOURCE_GIVEN:
        g_battle_effect_camera_position_target = *offset;
        break;
    case BATTLE_EFFECT_CAMERA_SOURCE_MAP_CENTRE:
        battle_effect_store_map_center_coordinates(&g_battle_effect_camera_position_target);
        if (offset) {
            battle_effect_add_vectors(
                offset, &g_battle_effect_camera_position_target, &g_battle_effect_camera_position_target);
        }
        break;
    case BATTLE_EFFECT_CAMERA_SOURCE_CURRENT:
        coords = battle_camera_get_current_real_coords();
        g_battle_effect_camera_position_target.vx = coords->vx >> 12;
        g_battle_effect_camera_position_target.vy = coords->vy >> 12;
        g_battle_effect_camera_position_target.vz = coords->vz >> 12;
        if (offset) {
            battle_effect_add_vectors(
                offset, &g_battle_effect_camera_position_target, &g_battle_effect_camera_position_target);
        }
        break;
    case BATTLE_EFFECT_CAMERA_SOURCE_SAVED:
        g_battle_effect_camera_position_target = g_battle_effect_camera_position_saved;
        if (offset) {
            battle_effect_add_vectors(
                offset, &g_battle_effect_camera_position_target, &g_battle_effect_camera_position_target);
        }
        break;
    case BATTLE_EFFECT_CAMERA_SOURCE_CASTER:
        if (g_battle_effect_context.targets[BATTLE_EFFECT_CASTER_ENTRY].target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
            battle_effect_copy_misc_unit_screen_location(
                g_battle_effect_context.targets[BATTLE_EFFECT_CASTER_ENTRY].data.unit.misc_id,
                &g_battle_effect_camera_position_target);
        } else {
            battle_effect_convert_tile_coords_to_world_coords(
                &g_battle_effect_context.targets[BATTLE_EFFECT_CASTER_ENTRY].data.tile,
                &g_battle_effect_camera_position_target);
        }
        if (offset) {
            battle_effect_add_vectors(
                offset, &g_battle_effect_camera_position_target, &g_battle_effect_camera_position_target);
        }
        break;
    case BATTLE_EFFECT_CAMERA_SOURCE_TARGET_AVERAGE:
        sum.vx = 0;
        sum.vy = 0;
        sum.vz = 0;
        for (i = 0; i < g_battle_effect_context.target_count; i++) {
            if (g_battle_effect_context.targets[i].target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
                screen
                    = battle_unit_get_screen_data_ptr_by_misc_id(g_battle_effect_context.targets[i].data.unit.misc_id);
                sum.vx += screen->vx;
                sum.vy += screen->vy;
                sum.vz += screen->vz;
            }
        }
        sum.vx /= i;
        sum.vy /= i;
        sum.vz /= i;
        if (offset) {
            battle_effect_add_vectors(offset, &sum, &g_battle_effect_camera_position_target);
        }
        break;
    case BATTLE_EFFECT_CAMERA_SOURCE_CURSOR_TILE:
        battle_effect_convert_tile_coords_to_world_coords(
            &g_battle_effect_target_tile.data.tile, &g_battle_effect_camera_position_target);
        if (offset) {
            battle_effect_add_vectors(
                offset, &g_battle_effect_camera_position_target, &g_battle_effect_camera_position_target);
        }
        break;
    default:
        return;
    }
    g_battle_effect_camera_position_mode = command & BATTLE_EFFECT_CAMERA_MODE_MASK;
    g_battle_effect_camera_position_duration = duration;
    g_battle_effect_camera_position_frame = 0;
}
