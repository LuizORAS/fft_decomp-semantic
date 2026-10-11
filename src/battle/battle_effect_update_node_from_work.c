#include "fft/battle.h"
#include "psx/types.h"

/* Copy a work record's position, heading, and colour onto its list node.
 *
 * With kind_flags bit 1 the velocity is rotated into camera space and its
 * screen heading stored on the node. Colour-curve records take each channel
 * from its animation curve at the record's curve frame; others use neutral
 * 0x80. */
void battle_effect_update_node_from_work(s16 record_index, effect_work_record_t* work) {
    SVECTOR direction;
    VECTOR rotated;

    work->data->x = work->motion.position.x >> 12;
    work->data->y = work->motion.position.y >> 12;
    work->data->z = work->motion.position.z >> 12;
    if (work->kind_flags & EFFECT_EMITTER_ORIENT_TO_VELOCITY) {
        SetRotMatrix(&g_battle_camera_matrix);
        SetTransMatrix(&g_battle_effect_particle_heading_matrix);
        direction.vx = work->motion.velocity.x >> 3;
        direction.vy = work->motion.velocity.y >> 3;
        direction.vz = work->motion.velocity.z >> 3;
        /* The flag word lands in the output vector's pad word; the target frame
         * has no separate flag slot. */
        RotTrans(&direction, &rotated, &rotated.pad);
        work->data->screen_rotation_angle = ratan2(rotated.vy, rotated.vx);
    }
    if (work->flags & EFFECT_EMITTER_FLAG_COLOUR_CURVES) {
        battle_effect_render_particle_sprite(work->data,
            g_battle_effect_animation_curve_data->curves[work->colour_curves[0]][work->curve_frame],
            g_battle_effect_animation_curve_data->curves[work->colour_curves[1]][work->curve_frame],
            g_battle_effect_animation_curve_data->curves[work->colour_curves[2]][work->curve_frame]);
    } else {
        battle_effect_render_particle_sprite(work->data, 0x80, 0x80, 0x80);
    }
}
