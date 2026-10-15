#include "fft/battle.h"
#include "psx/libc.h"
#include "psx/libgte.h"

/* Factor of the animation curve selected by a 1-based nibble (0 = none) for
 * the current frame column. The index is decremented in its own statement: a
 * folded `(nibble) - 1` lets CSE compare the nibble with zero instead of -1. */
#define CURVE_FACTOR(nibble)                                                                                           \
    curve = nibble;                                                                                                    \
    curve--;                                                                                                           \
    if (curve != -1) {                                                                                                 \
        factor = g_battle_effect_animation_curves[curve].factor[frame];                                                \
    } else {                                                                                                           \
        factor = 0;                                                                                                    \
    }

/* Inline form of battle_effect_get_random_between. */

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
 * The records returned by battle_effect_alloc_work_record are initialized through
 * effect_particle_physics_view_t. The lerps of the pair at 0x4c are computed
 * and discarded, as is the target's facing quadrant. */
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
    effect_geometry_entry_t* entry;
    effect_particle_physics_view_t* particle;
    s16* unit_coords;
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
    effect_particle_physics_view_t* head;
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

    entry = &g_battle_effect_geometry_table->entries[emitter_index].emitter;
    time = frame;
    frame = frame % EFFECT_ANIMATION_CURVE_FRAMES;
    kind = entry->placement_flags;
    flags = entry->flags;
    CURVE_FACTOR(entry->curve_indices[1] >> 24 & 0xf);
    if (time % battle_effect_lerp_linear(entry->spawn_interval_start, entry->spawn_interval_end, factor) != 0) {
        return;
    }

    SetTransMatrix(&g_battle_effect_emitter_matrix);
    target_index = g_battle_effect_state_records[record_index].target_index;
    rotate = (flags & EFFECT_EMITTER_VELOCITY_MASK) == EFFECT_EMITTER_VELOCITY_CASTER_FACING;
    battle_effect_resolve_target_render_coords(g_battle_effect_flags_section->flags & EFFECT_FLAG_TERRAIN_HEIGHT_ADJUST,
        BATTLE_EFFECT_TARGET_INDEX_CASTER, &g_battle_effect_coord_data, &caster);
    if (g_battle_effect_coord_data.targets[BATTLE_EFFECT_CASTER_ENTRY].target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
        facing = ((battle_unit_get_facing_quadrant_by_misc_id(
                       g_battle_effect_coord_data.targets[BATTLE_EFFECT_CASTER_ENTRY].id.misc_id)
                      + 1)
                     & 3)
            << 10;
        rotation.vz = 0;
        rotation.vx = 0;
        rotation.vy = facing;
        RotMatrix(&rotation, &matrix);
    }
    battle_effect_resolve_target_render_coords(g_battle_effect_flags_section->flags & EFFECT_FLAG_TERRAIN_HEIGHT_ADJUST,
        target_index, &g_battle_effect_coord_data, &target);
    if (g_battle_effect_coord_data.targets[target_index].target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
        battle_unit_get_facing_quadrant_by_misc_id(g_battle_effect_coord_data.targets[target_index].id.misc_id);
        unit_coords
            = battle_unit_get_event_offset_ptr_by_misc_id(g_battle_effect_coord_data.targets[target_index].id.misc_id);
    }
    tile = battle_map_get_tile_data_pointer(
        g_battle_effect_target_tile.id.tile_x, g_battle_effect_target_tile.tile_y, g_battle_effect_target_tile.map_z);

    CURVE_FACTOR(entry->curve_indices[1] >> 20 & 0xf);
    count = battle_effect_lerp_linear(entry->particle_count_start, entry->particle_count_end, factor);
    for (i = 0; i < count; i++) {
        particle = (effect_particle_physics_view_t*)battle_effect_alloc_work_record(
            record_index, (kind & EFFECT_WORK_KIND_MASK));
        particle->kind_flags = kind;
        particle->flags = flags;
        particle->curve_frame = 0;
        battle_effect_init_animation_sequence(particle->data, entry->animation_frame_group, entry->animation_index);
    }
    head = particle;

    CURVE_FACTOR(entry->curve_indices[0] & 0xf);
    battle_effect_interpolate_emitter_spawn_position(entry, factor, &spawn.vx);
    if (rotate) {
        SetRotMatrix(&matrix);
        rotation.vx = spawn.vx;
        rotation.vy = spawn.vy;
        rotation.vz = spawn.vz;
        RotTrans(&rotation, &spawn, &out.pad);
    }
    switch (kind & EFFECT_EMITTER_ORIGIN_MASK) {
    case EFFECT_EMITTER_ORIGIN_WORLD:
        break;
    case EFFECT_EMITTER_ORIGIN_CURSOR_TILE:
        spawn.vx += g_battle_effect_target_tile.id.tile_x * 28 + 14;
        spawn.vz += g_battle_effect_target_tile.tile_y * 28 + 14;
        spawn.vy -= tile->height * 12;
        break;
    case EFFECT_EMITTER_ORIGIN_CASTER:
        battle_effect_add_svector_to_vector(&caster.vx, &spawn.vx, &spawn.vx);
        break;
    case EFFECT_EMITTER_ORIGIN_TARGET:
        battle_effect_add_svector_to_vector(&target.vx, &spawn.vx, &spawn.vx);
        break;
    case EFFECT_EMITTER_ORIGIN_PARENT_PARTICLE:
        if (parent != 0) {
            spawn.vx += parent->position[0] >> 12;
            spawn.vy += parent->position[1] >> 12;
            spawn.vz += parent->position[2] >> 12;
        }
        break;
    case EFFECT_EMITTER_ORIGIN_MAP_CENTRE:
        battle_map_store_max_coordinates(&map_max);
        spawn.vx += map_max.vx * 14;
        spawn.vz += map_max.vz * 14;
        break;
    case EFFECT_EMITTER_ORIGIN_TRACKED_ENTITY:
        spawn.vx += target.vx + unit_coords[0];
        spawn.vy += target.vy + unit_coords[1];
        spawn.vz += target.vz + unit_coords[2];
        break;
    }

    CURVE_FACTOR(entry->curve_indices[0] >> 4 & 0xf);
    battle_effect_interpolate_emitter_spawn_spread(entry, factor, &spread.vx);
    if (rotate) {
        SetRotMatrix(&matrix);
        rotation.vx = spread.vx;
        rotation.vy = spread.vy;
        rotation.vz = spread.vz;
        RotTrans(&rotation, &spread, &out.pad);
    }
    particle = head;
    if (kind & EFFECT_EMITTER_BOX_SPREAD) {
        for (i = 0; i < count; i++) {
            particle->position[0] = (spawn.vx + RANDOM_BETWEEN(spread.vx, -spread.vx)) << 12;
            particle->position[1] = (spawn.vy + RANDOM_BETWEEN(spread.vy, -spread.vy)) << 12;
            particle->position[2] = (spawn.vz + RANDOM_BETWEEN(spread.vz, -spread.vz)) << 12;
            particle = particle->next;
        }
    } else {
        scale = battle_effect_select_largest_magnitude_s16(spread.vx, spread.vy, spread.vz);
        for (i = 0; i < count; i++) {
            if (scale != 0) {
                rotation.vx = rand();
                rotation.vy = rand();
                rotation.vz = rand();
                RotMatrix(&rotation, &g_battle_effect_emitter_matrix);
                SetRotMatrix(&g_battle_effect_emitter_matrix);
                rotation.vx = scale;
                rotation.vz = 0;
                rotation.vy = 0;
                RotTrans(&rotation, &out, &out.pad);
                particle->position[0] = ((out.vx * spread.vx) / scale + spawn.vx) << 12;
                particle->position[1] = ((out.vy * spread.vy) / scale + spawn.vy) << 12;
                particle->position[2] = ((out.vz * spread.vz) / scale + spawn.vz) << 12;
            } else {
                particle->position[0] = spawn.vx << 12;
                particle->position[1] = spawn.vy << 12;
                particle->position[2] = spawn.vz << 12;
            }
            particle = particle->next;
        }
    }

    curve = entry->curve_indices[0] >> 28;
    curve--;
    if (curve != -1) {
        factor = g_battle_effect_animation_curves[curve].factor[frame];
        speed_min = battle_effect_lerp_linear(entry->radial_speed_start[0], entry->radial_speed_end[0], factor);
        speed_max = battle_effect_lerp_linear(entry->radial_speed_start[1], entry->radial_speed_end[1], factor);
    } else {
        speed_min = entry->radial_speed_start[0];
        speed_max = entry->radial_speed_start[1];
    }
    curve = entry->curve_indices[0] >> 12 & 0xf;
    curve--;
    if (curve != -1) {
        factor = g_battle_effect_animation_curves[curve].factor[frame];
        angle_spread.vx
            = battle_effect_lerp_linear(entry->velocity_spread_start[0], entry->velocity_spread_end[0], factor);
        angle_spread.vy
            = battle_effect_lerp_linear(entry->velocity_spread_start[1], entry->velocity_spread_end[1], factor);
        angle_spread.vz
            = battle_effect_lerp_linear(entry->velocity_spread_start[2], entry->velocity_spread_end[2], factor);
    } else {
        angle_spread.vx = entry->velocity_spread_start[0] / 2;
        angle_spread.vy = entry->velocity_spread_start[1] / 2;
        angle_spread.vz = entry->velocity_spread_start[2] / 2;
    }
    CURVE_FACTOR(entry->curve_indices[0] >> 8 & 0xf);
    battle_effect_interpolate_emitter_velocity_base_angles(entry, factor, &angle.vx);

    particle = head;
    switch (flags & EFFECT_EMITTER_VELOCITY_MASK) {
    case EFFECT_EMITTER_VELOCITY_OUTWARD:
        for (i = 0; i < count; i++) {
            rotation.vx = angle.vx + RANDOM_BETWEEN(angle_spread.vx, -angle_spread.vx);
            rotation.vy = angle.vy + RANDOM_BETWEEN(angle_spread.vy, -angle_spread.vy);
            rotation.vz = angle.vz + RANDOM_BETWEEN(angle_spread.vz, -angle_spread.vz);
            RotMatrix(&rotation, &g_battle_effect_emitter_matrix);
            SetRotMatrix(&g_battle_effect_emitter_matrix);
            rotation.vz = 0;
            rotation.vx = 0;
            rotation.vy = RANDOM_BETWEEN(speed_min, speed_max);
            RotTrans(&rotation, &out, &out.pad);
            particle->velocity[0] = out.vx << 3;
            particle->velocity[1] = out.vy << 3;
            particle->velocity[2] = out.vz << 3;
            particle = particle->next;
        }
        break;
    case EFFECT_EMITTER_VELOCITY_CASTER_FACING:
        for (i = 0; i < count; i++) {
            rotation.vx = angle.vx + RANDOM_BETWEEN(angle_spread.vx, -angle_spread.vx);
            rotation.vy = angle.vy + RANDOM_BETWEEN(angle_spread.vy, -angle_spread.vy);
            rotation.vz = angle.vz + RANDOM_BETWEEN(angle_spread.vz, -angle_spread.vz);
            RotMatrix(&rotation, &g_battle_effect_emitter_matrix);
            SetRotMatrix(&g_battle_effect_emitter_matrix);
            rotation.vz = 0;
            rotation.vx = 0;
            rotation.vy = RANDOM_BETWEEN(speed_min, speed_max);
            RotTrans(&rotation, &out, &out.pad);
            SetRotMatrix(&matrix);
            rotation.vx = out.vx;
            rotation.vy = out.vy;
            rotation.vz = out.vz;
            RotTrans(&rotation, &out, &out.pad);
            particle->velocity[0] = out.vx << 3;
            particle->velocity[1] = out.vy << 3;
            particle->velocity[2] = out.vz << 3;
            particle = particle->next;
        }
        break;
    case EFFECT_EMITTER_VELOCITY_TOWARD_TARGET:
        break;
    case EFFECT_EMITTER_VELOCITY_INWARD:
        for (i = 0; i < count; i++) {
            out.vx = spawn.vx - (particle->position[0] >> 12);
            out.vy = spawn.vy - (particle->position[1] >> 12);
            out.vz = spawn.vz - (particle->position[2] >> 12);
            speed = RANDOM_BETWEEN(speed_min, speed_max);
            if (out.vx != 0 || out.vy != 0 || out.vz != 0) {
                VectorNormal(&out, &out);
                particle->velocity[0] = (speed * out.vx) >> 9;
                particle->velocity[1] = (speed * out.vy) >> 9;
                particle->velocity[2] = (speed * out.vz) >> 9;
            } else {
                particle->velocity[1] = speed << 3;
                particle->velocity[2] = 0;
                particle->velocity[0] = 0;
            }
            particle = particle->next;
        }
        break;
    }

    CURVE_FACTOR(entry->curve_indices[1] & 0xf);
    accel_min.vx = battle_effect_lerp_linear(entry->acceleration_start[0], entry->acceleration_end[0], factor);
    accel_max.vx = battle_effect_lerp_linear(entry->acceleration_start[1], entry->acceleration_end[1], factor);
    accel_min.vy = battle_effect_lerp_linear(entry->acceleration_start[2], entry->acceleration_end[2], factor);
    accel_max.vy = battle_effect_lerp_linear(entry->acceleration_start[3], entry->acceleration_end[3], factor);
    accel_min.vz = battle_effect_lerp_linear(entry->acceleration_start[4], entry->acceleration_end[4], factor);
    accel_max.vz = battle_effect_lerp_linear(entry->acceleration_start[5], entry->acceleration_end[5], factor);
    if (rotate) {
        SetRotMatrix(&matrix);
        RotTrans(&accel_min, &out, &out.pad);
        accel_min.vx = out.vx;
        accel_min.vy = out.vy;
        accel_min.vz = out.vz;
        RotTrans(&accel_max, &out, &out.pad);
        accel_max.vx = out.vx;
        accel_max.vy = out.vy;
        accel_max.vz = out.vz;
    }
    CURVE_FACTOR(entry->curve_indices[1] >> 4 & 0xf);
    accel_step_min.vx
        = battle_effect_lerp_linear(entry->acceleration_step_start[0], entry->acceleration_step_end[0], factor);
    accel_step_max.vx
        = battle_effect_lerp_linear(entry->acceleration_step_start[1], entry->acceleration_step_end[1], factor);
    accel_step_min.vy
        = battle_effect_lerp_linear(entry->acceleration_step_start[2], entry->acceleration_step_end[2], factor);
    accel_step_max.vy
        = battle_effect_lerp_linear(entry->acceleration_step_start[3], entry->acceleration_step_end[3], factor);
    accel_step_min.vz
        = battle_effect_lerp_linear(entry->acceleration_step_start[4], entry->acceleration_step_end[4], factor);
    accel_step_max.vz
        = battle_effect_lerp_linear(entry->acceleration_step_start[5], entry->acceleration_step_end[5], factor);
    if (rotate) {
        RotTrans(&accel_step_min, &out, &out.pad);
        accel_step_min.vx = out.vx;
        accel_step_min.vy = out.vy;
        accel_step_min.vz = out.vz;
        RotTrans(&accel_step_max, &out, &out.pad);
        accel_step_max.vx = out.vx;
        accel_step_max.vy = out.vy;
        accel_step_max.vz = out.vz;
    }
    particle = head;
    for (i = 0; i < count; i++) {
        particle->acceleration[0] = RANDOM_BETWEEN(accel_min.vx, accel_max.vx);
        particle->acceleration[1] = RANDOM_BETWEEN(accel_min.vy, accel_max.vy);
        particle->acceleration[2] = RANDOM_BETWEEN(accel_min.vz, accel_max.vz);
        particle->acceleration_step[0] = RANDOM_BETWEEN(accel_step_min.vx, accel_step_max.vx);
        particle->acceleration_step[1] = RANDOM_BETWEEN(accel_step_min.vy, accel_step_max.vy);
        particle->acceleration_step[2] = RANDOM_BETWEEN(accel_step_min.vz, accel_step_max.vz);
        particle = particle->next;
    }

    CURVE_FACTOR(entry->curve_indices[0] >> 16 & 0xf);
    inertia_min = battle_effect_lerp_linear(entry->inertia_start[0], entry->inertia_end[0], factor);
    inertia_max = battle_effect_lerp_linear(entry->inertia_start[1], entry->inertia_end[1], factor);
    CURVE_FACTOR(entry->curve_indices[0] >> 20 & 0xf);
    battle_effect_lerp_linear(entry->_unknown_4c[0], entry->_unknown_50[0], factor);
    battle_effect_lerp_linear(entry->_unknown_4c[1], entry->_unknown_50[1], factor);
    CURVE_FACTOR(entry->curve_indices[0] >> 24 & 0xf);
    weight_min = battle_effect_lerp_linear(entry->weight_start[0], entry->weight_end[0], factor);
    weight_max = battle_effect_lerp_linear(entry->weight_start[1], entry->weight_end[1], factor);
    CURVE_FACTOR(entry->curve_indices[1] >> 8 & 0xf);
    lifetime_min = battle_effect_lerp_linear(entry->lifetime_start[0], entry->lifetime_end[0], factor);
    lifetime_max = battle_effect_lerp_linear(entry->lifetime_start[1], entry->lifetime_end[1], factor);
    particle = head;
    for (i = 0; i < count; i++) {
        particle->inertia = RANDOM_BETWEEN(inertia_min, inertia_max);
        particle->weight = RANDOM_BETWEEN(weight_min, weight_max);
        particle->lifetime = RANDOM_BETWEEN(lifetime_min, lifetime_max);
        particle = particle->next;
    }

    CURVE_FACTOR(entry->curve_indices[1] >> 12 & 0xf);
    target_offset.vx = battle_effect_lerp_linear(entry->target_offset_start[0], entry->target_offset_end[0], factor);
    target_offset.vy = battle_effect_lerp_linear(entry->target_offset_start[1], entry->target_offset_end[1], factor);
    target_offset.vz = battle_effect_lerp_linear(entry->target_offset_start[2], entry->target_offset_end[2], factor);
    particle = head;
    switch (kind & EFFECT_EMITTER_HOMING_MASK) {
    case EFFECT_EMITTER_HOMING_OFFSET:
    case EFFECT_EMITTER_HOMING_OFFSET_ALT:
        for (i = 0; i < count; i++) {
            particle->target[0] = target_offset.vx;
            particle->target[1] = target_offset.vy;
            particle->target[2] = target_offset.vz;
            particle = particle->next;
        }
        break;
    case EFFECT_EMITTER_HOMING_MAP_CENTRE:
        battle_map_store_max_coordinates(&map_max);
        for (i = 0; i < count; i++) {
            particle->target[0] = map_max.vx * 14 + target_offset.vx;
            particle->target[2] = map_max.vz * 14 + target_offset.vz;
            particle->target[1] = target_offset.vy;
            particle = particle->next;
        }
        break;
    case EFFECT_EMITTER_HOMING_CASTER:
        for (i = 0; i < count; i++) {
            particle->target[0] = caster.vx + target_offset.vx;
            particle->target[1] = caster.vy + target_offset.vy;
            particle->target[2] = caster.vz + target_offset.vz;
            particle = particle->next;
        }
        break;
    case EFFECT_EMITTER_HOMING_TARGET:
        for (i = 0; i < count; i++) {
            particle->target[0] = target.vx + target_offset.vx;
            particle->target[1] = target.vy + target_offset.vy;
            particle->target[2] = target.vz + target_offset.vz;
            particle = particle->next;
        }
        break;
    case EFFECT_EMITTER_HOMING_CURSOR_TILE:
        tile_anchor.vx = g_battle_effect_target_tile.id.tile_x * 28 + 14;
        tile_anchor.vz = g_battle_effect_target_tile.tile_y * 28 + 14;
        tile_anchor.vy = -(tile->height * 12);
        for (i = 0; i < count; i++) {
            particle->target[0] = tile_anchor.vx + target_offset.vx;
            particle->target[1] = tile_anchor.vy + target_offset.vy;
            particle->target[2] = tile_anchor.vz + target_offset.vz;
            particle = particle->next;
        }
        break;
    }

    CURVE_FACTOR(entry->curve_indices[1] >> 28 & 3);
    /* The lerp results pass through curve: that reassignment keeps CSE from
     * sign-extending the call results instead of homing_min/max. */
    curve = battle_effect_lerp_linear(entry->homing_strength_start[0], entry->homing_strength_end[0], factor);
    homing_min = curve;
    curve = battle_effect_lerp_linear(entry->homing_strength_start[1], entry->homing_strength_end[1], factor);
    homing_max = curve;
    /* The dead test on word is required for the match: it keeps the word live
     * past the shift and splits the block before the loop setup. */
    word = entry->curve_indices[1];
    blend = word >> 30;
    if (word != 0) {
        curve = word >> 28 & 3;
    }
    colour_r = entry->colour_curves & 0xf;
    colour_g = entry->colour_curves >> 4 & 0xf;
    colour_b = entry->colour_curves >> 8 & 0xf;
    particle = head;
    for (i = 0; i < count; i++) {
        particle->homing_strength = RANDOM_BETWEEN(homing_min, homing_max);
        particle->homing_curve_index = blend;
        particle->colour_curves[0] = colour_r;
        particle->colour_curves[1] = colour_g;
        particle->colour_curves[2] = colour_b;
        if (flags & EFFECT_EMITTER_FLAG_CHILD_ON_DEATH_MASK) {
            particle->child_emitter_on_death = entry->child_emitter_on_death;
        }
        if (flags & EFFECT_EMITTER_FLAG_CHILD_MID_LIFE_MASK) {
            particle->child_emitter_mid_life = entry->child_emitter_mid_life;
        }
        particle = particle->next;
    }
}
