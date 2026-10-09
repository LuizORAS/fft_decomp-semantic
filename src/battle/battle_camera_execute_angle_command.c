#include "fft/battle.h"
#include "psx/types.h"

/* Timeline track 1 handler: sets the effect camera angle target and duration.
 *
 * Mode bits 0x1e0 pick the source: the best unblocked view of a target's or
 * the caster's tile (0x0, 0x140) or of the cursor tile (0x1c0), the given
 * angles with the yaw wrapped to the nearest turn (0x40), the current
 * rotation (0xc0), the current rotation plus the given offsets (0x20), the
 * saved pitch with the current yaw/roll (0x60) or the saved angles with the yaw
 * wrapped against g_battle_effect_camera_rotation_current (0x100). Most modes then add `angles` when
 * present. Bits 0x1e00 are kept in g_battle_effect_camera_rotation_mode; other modes return untouched.
 *
 * Mode 0x40 keeps its own `current` pointer: sharing `rotation` with the other
 * modes changes that case's register allocation. */
void battle_camera_execute_angle_command(s32 flags, s32 target, s32 duration, SVECTOR* angles) {
    SVECTOR coords;
    SVECTOR* rotation;
    SVECTOR* current;
    s16 delta;

    switch (flags & BATTLE_EFFECT_CAMERA_SOURCE_MASK) {
    case BATTLE_EFFECT_CAMERA_SOURCE_TARGET:
        if (g_battle_effect_coord_data.targets[target].target_type == 0) {
            battle_unit_get_map_coords_from_misc_id(g_battle_effect_coord_data.targets[target].id.misc_id, &coords.vx);
        } else {
            coords = *(SVECTOR*)&g_battle_effect_coord_data.targets[target].id;
        }
        battle_camera_choose_unblocked_rotation_for_tile(&coords, &g_battle_effect_camera_rotation_target);
        if (angles) {
            battle_effect_add_svectors(
                angles, &g_battle_effect_camera_rotation_target, &g_battle_effect_camera_rotation_target);
        }
        break;
    case BATTLE_EFFECT_CAMERA_SOURCE_GIVEN:
        current = battle_camera_get_rotation();
        delta = angles->vy - current->vy;
        if (delta < -0x800) {
            delta += ONE;
        } else if (delta > 0x800) {
            delta -= ONE;
        }
        g_battle_effect_camera_rotation_target.vx = angles->vx;
        g_battle_effect_camera_rotation_target.vy = delta + current->vy;
        g_battle_effect_camera_rotation_target.vz = angles->vz;
        g_battle_effect_camera_rotation_mode = flags & BATTLE_EFFECT_CAMERA_MODE_MASK;
        g_battle_effect_camera_rotation_duration = duration;
        g_battle_effect_camera_rotation_frame = 0;
        return;
    case BATTLE_EFFECT_CAMERA_SOURCE_CURRENT:
        rotation = battle_camera_get_rotation();
        g_battle_effect_camera_rotation_target.vx = rotation->vx;
        g_battle_effect_camera_rotation_target.vy = rotation->vy;
        g_battle_effect_camera_rotation_target.vz = rotation->vz;
        if (angles) {
            battle_effect_add_svectors(
                angles, &g_battle_effect_camera_rotation_target, &g_battle_effect_camera_rotation_target);
        }
        break;
    case BATTLE_EFFECT_CAMERA_SOURCE_SAVED:
        g_battle_effect_camera_rotation_target = g_battle_effect_camera_rotation_saved;
        if (angles) {
            battle_effect_add_svectors(
                angles, &g_battle_effect_camera_rotation_target, &g_battle_effect_camera_rotation_target);
        }
        delta = g_battle_effect_camera_rotation_target.vy - g_battle_effect_camera_rotation_current.vy;
        if (delta > 0x800) {
            g_battle_effect_camera_rotation_target.vy -= ONE;
        }
        if (delta < -0x800) {
            g_battle_effect_camera_rotation_target.vy += ONE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_SOURCE_CASTER:
        if (g_battle_effect_coord_data.targets[16].target_type == 0) {
            battle_unit_get_map_coords_from_misc_id(g_battle_effect_coord_data.targets[16].id.misc_id, &coords.vx);
        } else {
            coords = *(SVECTOR*)&g_battle_effect_coord_data.targets[16].id;
        }
        battle_camera_choose_unblocked_rotation_for_tile(&coords, &g_battle_effect_camera_rotation_target);
        if (angles) {
            battle_effect_add_svectors(
                angles, &g_battle_effect_camera_rotation_target, &g_battle_effect_camera_rotation_target);
        }
        break;
    case BATTLE_EFFECT_CAMERA_SOURCE_CURSOR_TILE:
        coords = *(SVECTOR*)&g_battle_effect_target_tile.id;
        battle_camera_choose_unblocked_rotation_for_tile(&coords, &g_battle_effect_camera_rotation_target);
        if (angles) {
            battle_effect_add_svectors(
                angles, &g_battle_effect_camera_rotation_target, &g_battle_effect_camera_rotation_target);
        }
        break;
    case BATTLE_EFFECT_CAMERA_SOURCE_CURRENT_YAW_ROLL:
        rotation = battle_camera_get_rotation();
        if (angles) {
            g_battle_effect_camera_rotation_target.vx = angles->vx;
            g_battle_effect_camera_rotation_target.vy = rotation->vy + angles->vy;
            g_battle_effect_camera_rotation_target.vz = rotation->vz + angles->vz;
        }
        break;
    case BATTLE_EFFECT_CAMERA_SOURCE_SAVED_PITCH:
        rotation = battle_camera_get_rotation();
        g_battle_effect_camera_rotation_target.vx = g_battle_effect_camera_rotation_saved.vx;
        g_battle_effect_camera_rotation_target.vy = rotation->vy;
        g_battle_effect_camera_rotation_target.vz = rotation->vz;
        if (angles) {
            battle_effect_add_svectors(
                angles, &g_battle_effect_camera_rotation_target, &g_battle_effect_camera_rotation_target);
        }
        break;
    default:
        return;
    }
    g_battle_effect_camera_rotation_mode = flags & BATTLE_EFFECT_CAMERA_MODE_MASK;
    g_battle_effect_camera_rotation_duration = duration;
    g_battle_effect_camera_rotation_frame = 0;
}
