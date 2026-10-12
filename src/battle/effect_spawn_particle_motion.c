#include "fft/battle_effect.h"
#include "fft/battle_unit.h"
#include "psx/libc.h"
#include "psx/libgpu.h"
#include "psx/libgte.h"

/* Spawns one particle's motion block from the interpolated emitter values and
 * returns its random lifetime.
 *
 * Position is the emitter offset plus a random point in the spread box, both
 * optionally rotated to face the caster or the target line (motion_flags 0x410
 * / 0x400) and anchored at the caster or target (anchor_flags 0x400 / 0x600).
 * Velocity is a random speed along the angle cone, or aimed back at the
 * position when motion_flags is 0x10. The caller stores the result as the
 * particle lifetime.
 *
 * The caster and target blocks go through a secondary-init record, as in the
 * secondary initializers; resolve_target_render_coords reads the same layout. */
s32 effect_spawn_particle_motion(effect_emitter_values_t* values, effect_motion_t* motion) {
    MATRIX anchor_matrix;
    MATRIX rotation;
    SVECTOR anchor_angle;
    SVECTOR vector;
    SVECTOR origin;
    SVECTOR target;
    VECTOR direction;
    VECTOR position;
    VECTOR spread;
    VECTOR angle;
    VECTOR half_spread;
    u8 _unused_c0[0x28]; /* reserved by the target frame, never referenced */
    effect_secondary_init_t init;
    s32 oriented;
    s16 scale;
    s16 speed_min;
    s16 speed_max;
    s16 speed;

    init.targets[EFFECT_CASTER_ENTRY] = g_effect_current_secondary->caster;
    effect_resolve_target_render_coords(0, EFFECT_TARGET_INDEX_CASTER, &init, &origin);
    init.targets[0] = g_effect_current_secondary->target.record;
    effect_resolve_target_render_coords(0, 0, &init, &target);
    oriented = 0;
    SetTransMatrix(&g_effect_projectile_matrix);
    if ((values->motion_flags & EFFECT_EMITTER_VELOCITY_MASK) == EFFECT_EMITTER_VELOCITY_CASTER_FACING) {
        oriented = 1;
        anchor_angle.vy
            = ((battle_unit_get_facing_quadrant_by_misc_id(g_effect_current_secondary->caster.data.unit.misc_id) + 1)
                  & 3)
            << 10;
        anchor_angle.vz = 0;
        anchor_angle.vx = 0;
        RotMatrix(&anchor_angle, &anchor_matrix);
    } else if ((values->motion_flags & EFFECT_EMITTER_VELOCITY_MASK) == EFFECT_EMITTER_VELOCITY_TOWARD_TARGET) {
        direction.vz = target.vz - origin.vz;
        direction.vx = target.vx - origin.vx;
        direction.vy = target.vy - origin.vy;
        oriented = 1;
        anchor_angle.vy = ratan2(-direction.vz, direction.vx);
        anchor_angle.vz
            = ratan2(direction.vy, SquareRoot0(direction.vx * direction.vx + direction.vz * direction.vz)) + 0x400;
        anchor_angle.vx = 0;
        RotMatrix(&anchor_angle, &anchor_matrix);
    }

    setVector(&position, values->position.x, values->position.y, values->position.z);
    if (oriented) {
        SetRotMatrix(&anchor_matrix);
        copyVector(&vector, &position);
        RotTrans(&vector, &position, &direction.pad);
    }
    switch (values->anchor_flags & EFFECT_EMITTER_ORIGIN_MASK) {
    case EFFECT_EMITTER_ORIGIN_CASTER:
        position.vx += origin.vx;
        position.vy += origin.vy;
        position.vz += origin.vz;
        break;
    case EFFECT_EMITTER_ORIGIN_TARGET:
        position.vx += target.vx;
        position.vy += target.vy;
        position.vz += target.vz;
        break;
    }

    setVector(&spread, values->spread.x, values->spread.y, values->spread.z);
    if (oriented) {
        copyVector(&vector, &spread);
        RotTrans(&vector, &spread, &direction.pad);
    }
    scale = effect_select_largest_magnitude_s16(spread.vx, spread.vy, spread.vz);
    setVector(&vector, rand(), rand(), rand());
    RotMatrix(&vector, &rotation);
    SetRotMatrix(&rotation);
    vector.vx = scale;
    vector.vz = 0;
    vector.vy = 0;
    RotTrans(&vector, &direction, &direction.pad);
    motion->position.x = ((direction.vx * spread.vx) / scale + position.vx) << 12;
    motion->position.y = ((direction.vy * spread.vy) / scale + position.vy) << 12;
    motion->position.z = ((direction.vz * spread.vz) / scale + position.vz) << 12;

    speed_min = values->speed_min;
    speed_max = values->speed_max;
    setVector(&angle, values->angle.x, values->angle.y, values->angle.z);
    setVector(&half_spread, values->angle_spread.x / 2, values->angle_spread.y / 2, values->angle_spread.z / 2);
    switch (values->motion_flags & EFFECT_EMITTER_VELOCITY_MASK) {
    case EFFECT_EMITTER_VELOCITY_OUTWARD:
    case EFFECT_EMITTER_VELOCITY_TOWARD_TARGET:
    case EFFECT_EMITTER_VELOCITY_CASTER_FACING:
        setVector(&vector, angle.vx + effect_get_random_between(half_spread.vx, -half_spread.vx),
            angle.vy + effect_get_random_between(half_spread.vy, -half_spread.vy),
            angle.vz + effect_get_random_between(half_spread.vz, -half_spread.vz));
        RotMatrix(&vector, &rotation);
        SetRotMatrix(&rotation);
        vector.vz = 0;
        vector.vx = 0;
        vector.vy = effect_get_random_between(speed_min, speed_max);
        RotTrans(&vector, &direction, &direction.pad);
        if (oriented) {
            copyVector(&vector, &direction);
            SetRotMatrix(&anchor_matrix);
            RotTrans(&vector, &direction, &direction.pad);
        }
        motion->velocity.x = direction.vx << 3;
        motion->velocity.y = direction.vy << 3;
        motion->velocity.z = direction.vz << 3;
        break;
    case EFFECT_EMITTER_VELOCITY_INWARD:
        setVector(&direction, position.vx - (motion->position.x >> 12), position.vy - (motion->position.y >> 12),
            position.vz - (motion->position.z >> 12));
        speed = effect_get_random_between(speed_min, speed_max);
        if (direction.vx != 0 || direction.vy != 0 || direction.vz != 0) {
            VectorNormal(&direction, &direction);
            motion->velocity.x = (speed * direction.vx) >> 9;
            motion->velocity.y = (speed * direction.vy) >> 9;
            motion->velocity.z = (speed * direction.vz) >> 9;
        } else {
            motion->velocity.y = speed << 3;
            motion->velocity.z = 0;
            motion->velocity.x = 0;
        }
        break;
    }
    motion->acceleration.x = motion->acceleration.y = motion->acceleration.z = motion->acceleration_step.x
        = motion->acceleration_step.y = motion->acceleration_step.z = 0;
    motion->inertia = ONE;
    motion->weight = effect_get_random_between(values->weight.min, values->weight.max);
    return effect_get_random_between(values->lifetime_min, values->lifetime_max);
}
