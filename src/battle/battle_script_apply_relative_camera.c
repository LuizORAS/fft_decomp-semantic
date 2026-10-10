#include "fft/battle.h"
#include "psx/types.h"

/* RelativeCamera: rewrite the next Camera instruction's seven operand halfwords
 * as the current camera values plus this instruction's deltas.
 *
 * src points at the seven camera script variables (x, z, y, tilt, yaw, roll,
 * zoom); the three positions are divided by 1024 first. A delta of
 * BATTLE_CAMERA_FUSION_HOLD_SENTINEL (10000) is written through unchanged, which
 * keeps that component. The Camera instruction is the one that follows, or the
 * one after a FocusSpeed that follows. */
void battle_script_apply_relative_camera(u8* parameters, s32* src) {
    u8* dst;
    s32 i;
    s32 delta;
    s32 val;

    dst = parameters + 0xF;
    if (parameters[0xE] == EVENT_OPCODE_FOCUS_SPEED) {
        dst = parameters + 0x12;
    }
    for (i = 0; i < 7; i++) {
        delta = *src;
        if (i < 3) {
            delta = delta / 1024;
        }
        if (battle_script_load_halfword(parameters) == BATTLE_CAMERA_FUSION_HOLD_SENTINEL) {
            val = BATTLE_CAMERA_FUSION_HOLD_SENTINEL;
        } else {
            val = delta + battle_script_load_halfword(parameters);
        }
        battle_script_store_halfword(dst, val);
        parameters += 2;
        dst += 2;
        src++;
    }
}
