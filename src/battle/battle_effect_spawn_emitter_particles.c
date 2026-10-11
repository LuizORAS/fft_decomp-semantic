#include "fft/battle.h"
#include "psx/libc.h"
#include "psx/libgte.h"

/* Spawns an emitter's particles for this frame and initializes their motion.
 *
 * Emitter emitter_index of the effect's particle-system section spawns every
 * spawn_interval frames of `frame`, which also picks the column of the
 * animation curves, modulo EFFECT_ANIMATION_CURVE_FRAMES. Each start/end pair
 * is lerped by the curve that its curve_indices nibble selects, and the
 * per-particle values are drawn between the resulting minimum and maximum.
 * The particles belong to state record record_index and are placed for its
 * target; parent is the particle that the PARENT_PARTICLE origin spawns from.
 *
 * The lerps of the pair at 0x4c and the target's facing quadrant are discarded. */
s32 battle_effect_spawn_emitter_particles(
    s16 record_index, s32 frame, s32 emitter_index, effect_work_record_t* parent) {
    SVECTOR rotation;
    VECTOR out;
    SVECTOR caster;
    MATRIX matrix;
    SVECTOR target;
    VECTOR spawn;
    VECTOR spread;
    VECTOR angle;
    VECTOR angle_spread;
    SVECTOR accel_min;
    SVECTOR accel_max;
    SVECTOR accel_step_min;
    SVECTOR accel_step_max;
    SVECTOR target_offset;
    SVECTOR map_max;
    SVECTOR tile_anchor;
    effect_geometry_record_t* entry;
    effect_work_record_t* particle;
    SVECTOR* unit_coords;
    s32 time;
    s32 curve;
    s32 factor;
    s32 i;
    u8 target_index;
    s16 scale;
    s16 speed_min;
    s16 speed_max;
    s16 speed;
    s16 facing;
    u32 word;
    s32 count;
    effect_work_record_t* head;
    u16 kind;
    u16 flags;
    s32 rotate;
    s16 inertia_min;
    s16 inertia_max;
    s16 weight_min;
    s16 weight_max;
    s16 lifetime_min;
    s16 lifetime_max;
    s16 homing_min;
    s16 homing_max;
    u16 blend;
    u8 colour_r;
    u8 colour_g;
    u8 colour_b;
    map_tile_t* tile;

    entry = &g_effect_geometry_table->entries[emitter_index];
    time = frame;
    frame = frame % EFFECT_ANIMATION_CURVE_FRAMES;
    kind = entry->emitter.placement_flags;
    flags = entry->emitter.flags;
    BATTLE_EFFECT_READ_CURVE(factor, curve, entry->emitter.curve_indices[1] >> 24 & 0xf, frame);
    if (time % battle_effect_lerp_linear(entry->emitter.spawn_interval_start, entry->emitter.spawn_interval_end, factor)
        != 0) {
        return;
    }

    SetTransMatrix(&g_battle_effect_emitter_matrix);
    target_index = g_effect_state_records[record_index].timeline.target_index;
    rotate = (flags & EFFECT_EMITTER_VELOCITY_MASK) == EFFECT_EMITTER_VELOCITY_CASTER_FACING;
    battle_effect_resolve_target_render_coords(g_battle_effect_flags_section->flags & EFFECT_FLAG_TERRAIN_HEIGHT_ADJUST,
        BATTLE_EFFECT_TARGET_INDEX_CASTER, &g_battle_effect_context, &caster);
    if (g_battle_effect_context.targets[BATTLE_EFFECT_CASTER_ENTRY].target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
        facing = ((battle_unit_get_facing_quadrant_by_misc_id(
                       g_battle_effect_context.targets[BATTLE_EFFECT_CASTER_ENTRY].data.unit.misc_id)
                      + 1)
                     & 3)
            << 10;
        rotation.vz = 0;
        rotation.vx = 0;
        rotation.vy = facing;
        RotMatrix(&rotation, &matrix);
    }
    battle_effect_resolve_target_render_coords(g_battle_effect_flags_section->flags & EFFECT_FLAG_TERRAIN_HEIGHT_ADJUST,
        target_index, &g_battle_effect_context, &target);
    if (g_battle_effect_context.targets[target_index].target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
        battle_unit_get_facing_quadrant_by_misc_id(g_battle_effect_context.targets[target_index].data.unit.misc_id);
        unit_coords = battle_unit_get_event_offset_ptr_by_misc_id(
            g_battle_effect_context.targets[target_index].data.unit.misc_id);
    }
    tile = battle_map_get_tile_data_pointer(g_battle_effect_target_tile.data.tile.vx,
        g_battle_effect_target_tile.data.tile.vz, g_battle_effect_target_tile.data.tile.vy);

    BATTLE_EFFECT_READ_CURVE(factor, curve, entry->emitter.curve_indices[1] >> 20 & 0xf, frame);
    count = battle_effect_lerp_linear(entry->emitter.particle_count_start, entry->emitter.particle_count_end, factor);
    for (i = 0; i < count; i++) {
        particle = battle_effect_alloc_work_record(record_index, (kind & EFFECT_WORK_KIND_MASK));
        particle->kind_flags = kind;
        particle->flags = flags;
        particle->curve_frame = 0;
        battle_effect_init_animation_sequence(
            particle->data, entry->emitter.animation_frame_group, entry->emitter.animation_index);
    }
    head = particle;

    BATTLE_EFFECT_READ_CURVE(factor, curve, entry->emitter.curve_indices[0] & 0xf, frame);
    battle_effect_interpolate_emitter_spawn_position(entry, factor, &spawn);
    if (rotate) {
        SetRotMatrix(&matrix);
        copyVector(&rotation, &spawn);
        RotTrans(&rotation, &spawn, &out.pad);
    }
    switch (kind & EFFECT_EMITTER_ORIGIN_MASK) {
    case EFFECT_EMITTER_ORIGIN_WORLD:
        break;
    case EFFECT_EMITTER_ORIGIN_CURSOR_TILE:
        spawn.vx += g_battle_effect_target_tile.data.tile.vx * 28 + 14;
        spawn.vz += g_battle_effect_target_tile.data.tile.vz * 28 + 14;
        spawn.vy -= tile->height * 12;
        break;
    case EFFECT_EMITTER_ORIGIN_CASTER:
        battle_effect_add_svector_to_vector(&caster, &spawn, &spawn);
        break;
    case EFFECT_EMITTER_ORIGIN_TARGET:
        battle_effect_add_svector_to_vector(&target, &spawn, &spawn);
        break;
    case EFFECT_EMITTER_ORIGIN_PARENT_PARTICLE:
        if (parent != 0) {
            spawn.vx += parent->motion.position.x >> 12;
            spawn.vy += parent->motion.position.y >> 12;
            spawn.vz += parent->motion.position.z >> 12;
        }
        break;
    case EFFECT_EMITTER_ORIGIN_MAP_CENTRE:
        battle_map_store_max_coordinates(&map_max);
        spawn.vx += map_max.vx * 14;
        spawn.vz += map_max.vz * 14;
        break;
    case EFFECT_EMITTER_ORIGIN_TRACKED_ENTITY:
        spawn.vx += target.vx + unit_coords->vx;
        spawn.vy += target.vy + unit_coords->vy;
        spawn.vz += target.vz + unit_coords->vz;
        break;
    }

    BATTLE_EFFECT_READ_CURVE(factor, curve, entry->emitter.curve_indices[0] >> 4 & 0xf, frame);
    battle_effect_interpolate_emitter_spawn_spread(entry, factor, &spread);
    if (rotate) {
        SetRotMatrix(&matrix);
        copyVector(&rotation, &spread);
        RotTrans(&rotation, &spread, &out.pad);
    }
    particle = head;
    if (kind & EFFECT_EMITTER_BOX_SPREAD) {
        for (i = 0; i < count; i++) {
            particle->motion.position.x = (spawn.vx + RANDOM_BETWEEN(spread.vx, -spread.vx)) << 12;
            particle->motion.position.y = (spawn.vy + RANDOM_BETWEEN(spread.vy, -spread.vy)) << 12;
            particle->motion.position.z = (spawn.vz + RANDOM_BETWEEN(spread.vz, -spread.vz)) << 12;
            particle = particle->next;
        }
    } else {
        scale = battle_effect_select_largest_magnitude_s16(spread.vx, spread.vy, spread.vz);
        for (i = 0; i < count; i++) {
            if (scale != 0) {
                setVector(&rotation, rand(), rand(), rand());
                RotMatrix(&rotation, &g_battle_effect_emitter_matrix);
                SetRotMatrix(&g_battle_effect_emitter_matrix);
                rotation.vx = scale;
                rotation.vz = 0;
                rotation.vy = 0;
                RotTrans(&rotation, &out, &out.pad);
                particle->motion.position.x = ((out.vx * spread.vx) / scale + spawn.vx) << 12;
                particle->motion.position.y = ((out.vy * spread.vy) / scale + spawn.vy) << 12;
                particle->motion.position.z = ((out.vz * spread.vz) / scale + spawn.vz) << 12;
            } else {
                particle->motion.position.x = spawn.vx << 12;
                particle->motion.position.y = spawn.vy << 12;
                particle->motion.position.z = spawn.vz << 12;
            }
            particle = particle->next;
        }
    }

    curve = entry->emitter.curve_indices[0] >> 28;
    curve--;
    if (curve != -1) {
        factor = g_battle_effect_animation_curve_data->curves[curve][frame];
        speed_min = battle_effect_lerp_linear(
            entry->emitter.radial_speed_start[0], entry->emitter.radial_speed_end[0], factor);
        speed_max = battle_effect_lerp_linear(
            entry->emitter.radial_speed_start[1], entry->emitter.radial_speed_end[1], factor);
    } else {
        speed_min = entry->emitter.radial_speed_start[0];
        speed_max = entry->emitter.radial_speed_start[1];
    }
    curve = entry->emitter.curve_indices[0] >> 12 & 0xf;
    curve--;
    if (curve != -1) {
        factor = g_battle_effect_animation_curve_data->curves[curve][frame];
        angle_spread.vx = battle_effect_lerp_linear(
            entry->emitter.velocity_spread_start.x, entry->emitter.velocity_spread_end.x, factor);
        angle_spread.vy = battle_effect_lerp_linear(
            entry->emitter.velocity_spread_start.y, entry->emitter.velocity_spread_end.y, factor);
        angle_spread.vz = battle_effect_lerp_linear(
            entry->emitter.velocity_spread_start.z, entry->emitter.velocity_spread_end.z, factor);
    } else {
        setVector(&angle_spread, entry->emitter.velocity_spread_start.x / 2, entry->emitter.velocity_spread_start.y / 2,
            entry->emitter.velocity_spread_start.z / 2);
    }
    BATTLE_EFFECT_READ_CURVE(factor, curve, entry->emitter.curve_indices[0] >> 8 & 0xf, frame);
    battle_effect_interpolate_emitter_velocity_base_angles(entry, factor, &angle);

    particle = head;
    switch (flags & EFFECT_EMITTER_VELOCITY_MASK) {
    case EFFECT_EMITTER_VELOCITY_OUTWARD:
        for (i = 0; i < count; i++) {
            setVector(&rotation, angle.vx + RANDOM_BETWEEN(angle_spread.vx, -angle_spread.vx),
                angle.vy + RANDOM_BETWEEN(angle_spread.vy, -angle_spread.vy),
                angle.vz + RANDOM_BETWEEN(angle_spread.vz, -angle_spread.vz));
            RotMatrix(&rotation, &g_battle_effect_emitter_matrix);
            SetRotMatrix(&g_battle_effect_emitter_matrix);
            rotation.vz = 0;
            rotation.vx = 0;
            rotation.vy = RANDOM_BETWEEN(speed_min, speed_max);
            RotTrans(&rotation, &out, &out.pad);
            particle->motion.velocity.x = out.vx << 3;
            particle->motion.velocity.y = out.vy << 3;
            particle->motion.velocity.z = out.vz << 3;
            particle = particle->next;
        }
        break;
    case EFFECT_EMITTER_VELOCITY_CASTER_FACING:
        for (i = 0; i < count; i++) {
            setVector(&rotation, angle.vx + RANDOM_BETWEEN(angle_spread.vx, -angle_spread.vx),
                angle.vy + RANDOM_BETWEEN(angle_spread.vy, -angle_spread.vy),
                angle.vz + RANDOM_BETWEEN(angle_spread.vz, -angle_spread.vz));
            RotMatrix(&rotation, &g_battle_effect_emitter_matrix);
            SetRotMatrix(&g_battle_effect_emitter_matrix);
            rotation.vz = 0;
            rotation.vx = 0;
            rotation.vy = RANDOM_BETWEEN(speed_min, speed_max);
            RotTrans(&rotation, &out, &out.pad);
            SetRotMatrix(&matrix);
            copyVector(&rotation, &out);
            RotTrans(&rotation, &out, &out.pad);
            particle->motion.velocity.x = out.vx << 3;
            particle->motion.velocity.y = out.vy << 3;
            particle->motion.velocity.z = out.vz << 3;
            particle = particle->next;
        }
        break;
    case EFFECT_EMITTER_VELOCITY_TOWARD_TARGET:
        break;
    case EFFECT_EMITTER_VELOCITY_INWARD:
        for (i = 0; i < count; i++) {
            setVector(&out, spawn.vx - (particle->motion.position.x >> 12),
                spawn.vy - (particle->motion.position.y >> 12), spawn.vz - (particle->motion.position.z >> 12));
            speed = RANDOM_BETWEEN(speed_min, speed_max);
            if (out.vx != 0 || out.vy != 0 || out.vz != 0) {
                VectorNormal(&out, &out);
                particle->motion.velocity.x = (speed * out.vx) >> 9;
                particle->motion.velocity.y = (speed * out.vy) >> 9;
                particle->motion.velocity.z = (speed * out.vz) >> 9;
            } else {
                particle->motion.velocity.y = speed << 3;
                particle->motion.velocity.z = 0;
                particle->motion.velocity.x = 0;
            }
            particle = particle->next;
        }
        break;
    }

    BATTLE_EFFECT_READ_CURVE(factor, curve, entry->emitter.curve_indices[1] & 0xf, frame);
    accel_min.vx
        = battle_effect_lerp_linear(entry->emitter.acceleration_start[0], entry->emitter.acceleration_end[0], factor);
    accel_max.vx
        = battle_effect_lerp_linear(entry->emitter.acceleration_start[1], entry->emitter.acceleration_end[1], factor);
    accel_min.vy
        = battle_effect_lerp_linear(entry->emitter.acceleration_start[2], entry->emitter.acceleration_end[2], factor);
    accel_max.vy
        = battle_effect_lerp_linear(entry->emitter.acceleration_start[3], entry->emitter.acceleration_end[3], factor);
    accel_min.vz
        = battle_effect_lerp_linear(entry->emitter.acceleration_start[4], entry->emitter.acceleration_end[4], factor);
    accel_max.vz
        = battle_effect_lerp_linear(entry->emitter.acceleration_start[5], entry->emitter.acceleration_end[5], factor);
    if (rotate) {
        SetRotMatrix(&matrix);
        RotTrans(&accel_min, &out, &out.pad);
        copyVector(&accel_min, &out);
        RotTrans(&accel_max, &out, &out.pad);
        copyVector(&accel_max, &out);
    }
    BATTLE_EFFECT_READ_CURVE(factor, curve, entry->emitter.curve_indices[1] >> 4 & 0xf, frame);
    accel_step_min.vx = battle_effect_lerp_linear(
        entry->emitter.acceleration_step_start[0], entry->emitter.acceleration_step_end[0], factor);
    accel_step_max.vx = battle_effect_lerp_linear(
        entry->emitter.acceleration_step_start[1], entry->emitter.acceleration_step_end[1], factor);
    accel_step_min.vy = battle_effect_lerp_linear(
        entry->emitter.acceleration_step_start[2], entry->emitter.acceleration_step_end[2], factor);
    accel_step_max.vy = battle_effect_lerp_linear(
        entry->emitter.acceleration_step_start[3], entry->emitter.acceleration_step_end[3], factor);
    accel_step_min.vz = battle_effect_lerp_linear(
        entry->emitter.acceleration_step_start[4], entry->emitter.acceleration_step_end[4], factor);
    accel_step_max.vz = battle_effect_lerp_linear(
        entry->emitter.acceleration_step_start[5], entry->emitter.acceleration_step_end[5], factor);
    if (rotate) {
        RotTrans(&accel_step_min, &out, &out.pad);
        copyVector(&accel_step_min, &out);
        RotTrans(&accel_step_max, &out, &out.pad);
        copyVector(&accel_step_max, &out);
    }
    particle = head;
    for (i = 0; i < count; i++) {
        particle->motion.acceleration.x = RANDOM_BETWEEN(accel_min.vx, accel_max.vx);
        particle->motion.acceleration.y = RANDOM_BETWEEN(accel_min.vy, accel_max.vy);
        particle->motion.acceleration.z = RANDOM_BETWEEN(accel_min.vz, accel_max.vz);
        particle->motion.acceleration_step.x = RANDOM_BETWEEN(accel_step_min.vx, accel_step_max.vx);
        particle->motion.acceleration_step.y = RANDOM_BETWEEN(accel_step_min.vy, accel_step_max.vy);
        particle->motion.acceleration_step.z = RANDOM_BETWEEN(accel_step_min.vz, accel_step_max.vz);
        particle = particle->next;
    }

    BATTLE_EFFECT_READ_CURVE(factor, curve, entry->emitter.curve_indices[0] >> 16 & 0xf, frame);
    inertia_min = battle_effect_lerp_linear(entry->emitter.inertia_start[0], entry->emitter.inertia_end[0], factor);
    inertia_max = battle_effect_lerp_linear(entry->emitter.inertia_start[1], entry->emitter.inertia_end[1], factor);
    BATTLE_EFFECT_READ_CURVE(factor, curve, entry->emitter.curve_indices[0] >> 20 & 0xf, frame);
    battle_effect_lerp_linear(entry->emitter._unknown_4c[0], entry->emitter._unknown_50[0], factor);
    battle_effect_lerp_linear(entry->emitter._unknown_4c[1], entry->emitter._unknown_50[1], factor);
    BATTLE_EFFECT_READ_CURVE(factor, curve, entry->emitter.curve_indices[0] >> 24 & 0xf, frame);
    weight_min = battle_effect_lerp_linear(entry->emitter.weight_start[0], entry->emitter.weight_end[0], factor);
    weight_max = battle_effect_lerp_linear(entry->emitter.weight_start[1], entry->emitter.weight_end[1], factor);
    BATTLE_EFFECT_READ_CURVE(factor, curve, entry->emitter.curve_indices[1] >> 8 & 0xf, frame);
    lifetime_min = battle_effect_lerp_linear(entry->emitter.lifetime_start[0], entry->emitter.lifetime_end[0], factor);
    lifetime_max = battle_effect_lerp_linear(entry->emitter.lifetime_start[1], entry->emitter.lifetime_end[1], factor);
    particle = head;
    for (i = 0; i < count; i++) {
        particle->motion.inertia = RANDOM_BETWEEN(inertia_min, inertia_max);
        particle->motion.weight = RANDOM_BETWEEN(weight_min, weight_max);
        particle->lifetime = RANDOM_BETWEEN(lifetime_min, lifetime_max);
        particle = particle->next;
    }

    BATTLE_EFFECT_READ_CURVE(factor, curve, entry->emitter.curve_indices[1] >> 12 & 0xf, frame);
    target_offset.vx
        = battle_effect_lerp_linear(entry->emitter.target_offset_start[0], entry->emitter.target_offset_end[0], factor);
    target_offset.vy
        = battle_effect_lerp_linear(entry->emitter.target_offset_start[1], entry->emitter.target_offset_end[1], factor);
    target_offset.vz
        = battle_effect_lerp_linear(entry->emitter.target_offset_start[2], entry->emitter.target_offset_end[2], factor);
    particle = head;
    switch (kind & EFFECT_EMITTER_HOMING_MASK) {
    case EFFECT_EMITTER_HOMING_OFFSET:
    case EFFECT_EMITTER_HOMING_OFFSET_ALT:
        for (i = 0; i < count; i++) {
            particle->target.x = target_offset.vx;
            particle->target.y = target_offset.vy;
            particle->target.z = target_offset.vz;
            particle = particle->next;
        }
        break;
    case EFFECT_EMITTER_HOMING_MAP_CENTRE:
        battle_map_store_max_coordinates(&map_max);
        for (i = 0; i < count; i++) {
            particle->target.x = map_max.vx * 14 + target_offset.vx;
            particle->target.z = map_max.vz * 14 + target_offset.vz;
            particle->target.y = target_offset.vy;
            particle = particle->next;
        }
        break;
    case EFFECT_EMITTER_HOMING_CASTER:
        for (i = 0; i < count; i++) {
            particle->target.x = caster.vx + target_offset.vx;
            particle->target.y = caster.vy + target_offset.vy;
            particle->target.z = caster.vz + target_offset.vz;
            particle = particle->next;
        }
        break;
    case EFFECT_EMITTER_HOMING_TARGET:
        for (i = 0; i < count; i++) {
            particle->target.x = target.vx + target_offset.vx;
            particle->target.y = target.vy + target_offset.vy;
            particle->target.z = target.vz + target_offset.vz;
            particle = particle->next;
        }
        break;
    case EFFECT_EMITTER_HOMING_CURSOR_TILE:
        tile_anchor.vx = g_battle_effect_target_tile.data.tile.vx * 28 + 14;
        tile_anchor.vz = g_battle_effect_target_tile.data.tile.vz * 28 + 14;
        tile_anchor.vy = -(tile->height * 12);
        for (i = 0; i < count; i++) {
            particle->target.x = tile_anchor.vx + target_offset.vx;
            particle->target.y = tile_anchor.vy + target_offset.vy;
            particle->target.z = tile_anchor.vz + target_offset.vz;
            particle = particle->next;
        }
        break;
    }

    BATTLE_EFFECT_READ_CURVE(factor, curve, entry->emitter.curve_indices[1] >> 28 & 3, frame);
    /* The lerp results pass through curve: that reassignment keeps CSE from
     * sign-extending the call results instead of homing_min/max. */
    curve = battle_effect_lerp_linear(
        entry->emitter.homing_strength_start[0], entry->emitter.homing_strength_end[0], factor);
    homing_min = curve;
    curve = battle_effect_lerp_linear(
        entry->emitter.homing_strength_start[1], entry->emitter.homing_strength_end[1], factor);
    homing_max = curve;
    /* The dead test on word is required for the match: it keeps the word live
     * past the shift and splits the block before the loop setup. */
    word = entry->emitter.curve_indices[1];
    blend = word >> 30;
    if (word != 0) {
        curve = word >> 28 & 3;
    }
    colour_r = entry->emitter.colour_curves & 0xf;
    colour_g = entry->emitter.colour_curves >> 4 & 0xf;
    colour_b = entry->emitter.colour_curves >> 8 & 0xf;
    particle = head;
    for (i = 0; i < count; i++) {
        particle->homing_strength = RANDOM_BETWEEN(homing_min, homing_max);
        particle->homing_curve_index = blend;
        particle->colour_curves[0] = colour_r;
        particle->colour_curves[1] = colour_g;
        particle->colour_curves[2] = colour_b;
        if (flags & EFFECT_EMITTER_FLAG_CHILD_ON_DEATH_MASK) {
            particle->child_emitter_on_death = entry->emitter.child_emitter_on_death;
        }
        if (flags & EFFECT_EMITTER_FLAG_CHILD_MID_LIFE_MASK) {
            particle->child_emitter_mid_life = entry->emitter.child_emitter_mid_life;
        }
        particle = particle->next;
    }
}
