#include "fft/battle.h"
#include "psx/types.h"

s16 battle_camera_pick_nearest_unblocked_yaw(s32 misc_id_a, s32 misc_id_b) {
    s32 yaw;
    s32 mask;
    s32 raw;
    /* Pin: unpinned, `bit` loses priority to raw/ptr/i and the four rotate one register. */
    register s32 bit __asm__("$7");
    s32 best;
    s32 sign;
    s32 i;
    s32 delta;
    s32 wrapped;
    s32 dist;

    yaw = battle_camera_wrap_yaw_angle() & 0xFFF;
    battle_unit_copy_map_coords_from_misc_id(misc_id_a, g_battle_scratch_coords);
    /* The raw result is assigned before it is masked: combine then knows
     * nothing about the mask's high bits and the loop test keeps its sll 16. */
    mask = battle_map_get_tile_data_value(
        0xB, g_battle_scratch_coords[0], g_battle_scratch_coords[2], g_battle_scratch_coords[1]);
    mask &= 0xFF;
    battle_unit_copy_map_coords_from_misc_id(misc_id_b, g_battle_scratch_coords);
    mask = (mask
               | battle_map_get_tile_data_value(
                   0xB, g_battle_scratch_coords[0], g_battle_scratch_coords[2], g_battle_scratch_coords[1]))
        & 0xF;
    bit = 1;
    best = 0x8000;
    sign = 1;
    for (i = 0; i < 4; i++) {
        if ((s16)(bit & mask) == 0) {
            delta = g_battle_camera_diagonal_yaws[i] - yaw;
            raw = delta;
            if (delta > 0) {
                wrapped = delta - 0x1000;
            } else {
                wrapped = delta + 0x1000;
            }
            if (delta < 0) {
                delta = -delta;
            }
            if (wrapped < 0) {
                wrapped = -wrapped;
            }
            /* Barrier: keeps reorg from stealing `dist = delta` into the bgez delay slot above. */
            __asm__ volatile("");

            if (wrapped < delta) {
                dist = wrapped;
            } else {
                dist = delta;
            }
            if (dist < best) {
                best = dist;
                if (raw > 0) {
                    sign = -1;
                    if (delta < wrapped) {
                        sign = 1;
                    }
                } else {
                    sign = 1;
                    if (delta < wrapped) {
                        sign = -1;
                    }
                }
            }
        }
        bit <<= 1;
    }
    return g_battle_script_variables[EVENT_SCRIPT_VAR_CAMERA_YAW] + sign * best;
}
