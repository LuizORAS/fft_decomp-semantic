#include "fft/battle_unit.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Distortion 1 (BATTLE_DISTORTION_ANIMATION_SCALE_PULSE, queued by no SEQ file): narrow the sprite to
 * nothing while stretching it up to 2.5 times its height and back, over about 40 frames, then
 * restore its scale and end. distortion_timer and distortion_target hold the two sine angles.
 *
 * Falls off the end: the original returns int, so $v0 stays live at the exit and the
 * dispatch branch's delay slot is left empty. Nothing reads the result: the distortion
 * dispatcher calls its handlers as void. */
s32 battle_unit_animate_squeeze_out(battle_unit_t* unit) {
    s32 angle;
    s32 fade;

    switch (unit->distortion_phase) {
    case 0:
        unit->distortion_phase = 1;
        unit->distortion_timer = 0x300;
        unit->distortion_target = 0;
    case 1:
        angle = unit->distortion_target;
        unit->animation.display->scale_x = rsin(unit->distortion_timer);
        fade = rsin(angle);
        unit->animation.display->scale_y = fade + fade / 2 + ONE;
        if ((unit->distortion_timer & 0xfff) != 0x800) {
            unit->distortion_timer = unit->distortion_timer + 32;
            unit->distortion_target = unit->distortion_target + 51;
        } else {
            unit->distortion_phase = unit->distortion_phase + 1;
        }
        break;
    case 2:
        unit->distortion_animation_id = BATTLE_DISTORTION_ANIMATION_NONE;
        unit->animation.display->scale_x = ONE;
        unit->animation.display->scale_y = ONE;
        break;
    }
}
