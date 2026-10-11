#include "fft/battle.h"
#include "psx/types.h"

/* Integrate one frame of motion for every particle in a list.
 *
 * Applies motion integration to each node. A node with no
 * homing strength then adds its acceleration step to its acceleration. One
 * with a strength adds delta + (step - delta) * s / 127 instead, where delta
 * is the unit vector to its homing target scaled by the strength and s is
 * its homing curve's factor less 0x80: a factor of 0xff adds the step alone,
 * 0x80 the target vector alone, and 0, or no curve, twice the target vector
 * less the step. The list parameter doubles as the next-node cursor: that
 * keeps both loop tests on it. */
void battle_effect_integrate_particle_motion(effect_work_record_t* list) {
    effect_work_record_t* node;
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

        motion->position.x += motion->velocity.x;
        motion->position.y += motion->velocity.y;
        motion->position.z += motion->velocity.z;

        weight = motion->inertia - threshold;

        motion->velocity.x = (weight * motion->velocity.x + (motion->acceleration.x << 12)) / motion->inertia
            + ((gravity_x * motion->weight) >> 12);
        motion->velocity.y = (weight * motion->velocity.y + (motion->acceleration.y << 12)) / motion->inertia
            + ((gravity_y * motion->weight) >> 12);
        motion->velocity.z = (weight * motion->velocity.z + (motion->acceleration.z << 12)) / motion->inertia
            + ((gravity_z * motion->weight) >> 12);

        weight = node->homing_strength;
        if (weight != 0) {
            track = node->homing_curve_index;
            track--;
            if (track != -1) {
                strength = g_battle_effect_animation_curve_data->curves[track][node->curve_frame] - 0x80;
            } else {
                strength = -0x80;
            }

            setVector(&delta, node->target.x - (motion->position.x >> 12), node->target.y - (motion->position.y >> 12),
                node->target.z - (motion->position.z >> 12));
            if (delta.vx != 0 || delta.vy != 0 || delta.vz != 0) {
                VectorNormal(&delta, &delta);
                setVector(&delta, (weight * delta.vx) >> 12, (weight * delta.vy) >> 12, (weight * delta.vz) >> 12);
            }

            motion->acceleration.x += ((motion->acceleration_step.x - delta.vx) * strength) / 127 + delta.vx;
            motion->acceleration.y += ((motion->acceleration_step.y - delta.vy) * strength) / 127 + delta.vy;
            motion->acceleration.z += ((motion->acceleration_step.z - delta.vz) * strength) / 127 + delta.vz;
        } else {
            motion->acceleration.x += motion->acceleration_step.x;
            motion->acceleration.y += motion->acceleration_step.y;
            motion->acceleration.z += motion->acceleration_step.z;
        }
        node = list;
    }
}
