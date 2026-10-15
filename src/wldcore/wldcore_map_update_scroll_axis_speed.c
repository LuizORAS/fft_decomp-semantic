#include "fft/wldcore.h"

/* Step one axis of the map cursor's accelerating move: direction 1 or -1 adds `step` toward the
 * signed `limit`, 0 lets the value decay toward rest by `decay`; `output` is the whole-pixel speed
 * (value >> 8).
 *
 * The negative side mirrors the positive one through negation rather than a
 * signed division, so the exported speed truncates toward zero on both sides. */
void wldcore_map_update_scroll_axis_speed(wldcore_axis_state_t* axis, s32 direction) {
    switch (direction) {
    case 1:
        if (axis->value < axis->limit) {
            axis->value = axis->value + axis->step;
        }
        if (axis->value > axis->limit) {
            axis->value = axis->limit;
        }
        axis->output = axis->value >> 8;
        break;
    case 0:
        if (axis->value == 0) {
            break;
        }
        if (axis->value > 0) {
            axis->value = axis->value - axis->decay;
            if (axis->value <= 0) {
                axis->value = 0;
            }
            axis->output = axis->value >> 8;
        } else {
            axis->value = axis->value + axis->decay;
            if (axis->value >= 0) {
                axis->value = 0;
            }
            axis->output = -((-axis->value) >> 8);
        }
        break;
    case -1:
        if (-axis->value < axis->limit) {
            axis->value = axis->value - axis->step;
        }
        if (-axis->value > axis->limit) {
            axis->value = -axis->limit;
        }
        axis->output = -((-axis->value) >> 8);
        break;
    }
}
