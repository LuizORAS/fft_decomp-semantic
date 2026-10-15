#include "fft/battle.h"
#include "psx/types.h"

/* The members of effect_work_record_t the integrator reads, with its motion
 * state as one battle_effect_motion_t. */
typedef struct battle_effect_particle {
    u8 _unused_00[4];                    /* 0x00 */
    struct battle_effect_particle* next; /* 0x04 */
    battle_effect_motion_t motion;       /* 0x08 */
    s16 target_x;                        /* 0x3c */
    s16 target_y;                        /* 0x3e */
    s16 target_z;                        /* 0x40 */
    s16 lifetime;                        /* 0x42 */
    u8 _unused_44;                       /* 0x44 */
    u8 homing_curve_index;               /* 0x45 */
    u8 _unused_46[4];                    /* 0x46 */
    s16 homing_strength;                 /* 0x4a */
    u8 _unused_4c[4];                    /* 0x4c */
    s16 curve_frame;                     /* 0x50 */
} battle_effect_particle_t;

/* Integrate one frame of motion for every particle in a list.
 *
 * Runs the battle_effect_step_motion integrator on each node. A node with no
 * homing strength then adds its acceleration step to its acceleration. One
 * with a strength adds delta + (step - delta) * s / 127 instead, where delta
 * is the unit vector to its homing target scaled by the strength and s is
 * its homing curve's factor less 0x80: a factor of 0xff adds the step alone,
 * 0x80 the target vector alone, and 0, or no curve, twice the target vector
 * less the step. The list parameter doubles as the next-node cursor: that
 * keeps both loop tests on it. */
void battle_effect_integrate_particle_motion(battle_effect_particle_t* list) {
    battle_effect_particle_t* node;
    battle_effect_motion_t* motion;
    s32 weight;
    s32 strength;
    s32 track;
    VECTOR delta;
    s32 threshold;
    s32 gravity_x;
    s32 gravity_y;
    s32 gravity_z;

    threshold = g_battle_effect_inertia_threshold;
    gravity_x = g_battle_effect_gravity.x;
    gravity_y = g_battle_effect_gravity.y;
    gravity_z = g_battle_effect_gravity.z;

    node = list;
    while (list != 0) {
        list = node->next;
        motion = &node->motion;

        motion->position[0] += motion->velocity[0];
        motion->position[1] += motion->velocity[1];
        motion->position[2] += motion->velocity[2];

        weight = motion->inertia - threshold;

        motion->velocity[0] = (weight * motion->velocity[0] + (motion->acceleration[0] << 12)) / motion->inertia
            + ((gravity_x * motion->weight) >> 12);
        motion->velocity[1] = (weight * motion->velocity[1] + (motion->acceleration[1] << 12)) / motion->inertia
            + ((gravity_y * motion->weight) >> 12);
        motion->velocity[2] = (weight * motion->velocity[2] + (motion->acceleration[2] << 12)) / motion->inertia
            + ((gravity_z * motion->weight) >> 12);

        weight = node->homing_strength;
        if (weight != 0) {
            track = node->homing_curve_index;
            track--;
            if (track != -1) {
                strength = g_battle_effect_animation_curves[track].factor[node->curve_frame] - 0x80;
            } else {
                strength = -0x80;
            }

            delta.vx = node->target_x - (motion->position[0] >> 12);
            delta.vy = node->target_y - (motion->position[1] >> 12);
            delta.vz = node->target_z - (motion->position[2] >> 12);
            if (delta.vx != 0 || delta.vy != 0 || delta.vz != 0) {
                VectorNormal(&delta, &delta);
                delta.vx = (weight * delta.vx) >> 12;
                delta.vy = (weight * delta.vy) >> 12;
                delta.vz = (weight * delta.vz) >> 12;
            }

            motion->acceleration[0] += ((motion->acceleration_step[0] - delta.vx) * strength) / 127 + delta.vx;
            motion->acceleration[1] += ((motion->acceleration_step[1] - delta.vy) * strength) / 127 + delta.vy;
            motion->acceleration[2] += ((motion->acceleration_step[2] - delta.vz) * strength) / 127 + delta.vz;
        } else {
            motion->acceleration[0] += motion->acceleration_step[0];
            motion->acceleration[1] += motion->acceleration_step[1];
            motion->acceleration[2] += motion->acceleration_step[2];
        }
        node = list;
    }
}
