#include "fft/battle_camera.h"
#include "fft/battle_map.h"
#include "fft/script.h"
#include "fft/world.h"
#include "psx/types.h"

/*
 * Picks the camera yaw for a shot of two units: of the four candidate yaws
 * in g_world_camera_candidate_yaws, the one nearest the current yaw whose bit
 * is clear in the flat camera-block masks of both units' tiles.
 *
 * Returns the camera-yaw script variable moved by the shorter signed arc to
 * that candidate (angles are 4096 to the turn). With all four candidates
 * blocked nothing replaces the initial distance and the result is the
 * variable plus 0x8000.
 */
s16 world_camera_pick_nearest_unblocked_yaw(s32 runtime_id_a, s32 runtime_id_b) {
    s32 yaw;
    s32 mask;
    s32 raw;
    /* The target allocates the mask bit to $7, ahead of raw/ptr/i; without
     * the binding GCC shifts all four up one slot in REG_ALLOC_ORDER. The
     * pin is safe because bit is live only after the last call. */
    register s32 bit __asm__("$7");
    s32 best;
    s32 sign;
    s32 i;
    s32 delta;
    s32 wrapped;
    s32 dist;

    yaw = camera_wrap_yaw_angle() & 0xFFF;
    world_unit_get_map_coords_by_runtime_id(runtime_id_a, &g_world_text_speaker_screen_coords);
    /* The raw result is assigned before it is masked: combine then knows
     * nothing about the mask's high bits and the loop test keeps its sll 16. */
    mask = map_get_tile_data_value(MAP_TILE_DATA_FLAT_CAMERA_BLOCK_MASK, g_world_text_speaker_screen_coords.vx,
        g_world_text_speaker_screen_coords.vz, g_world_text_speaker_screen_coords.vy);
    mask &= 0xFF;
    world_unit_get_map_coords_by_runtime_id(runtime_id_b, &g_world_text_speaker_screen_coords);
    mask = (mask
               | map_get_tile_data_value(MAP_TILE_DATA_FLAT_CAMERA_BLOCK_MASK, g_world_text_speaker_screen_coords.vx,
                   g_world_text_speaker_screen_coords.vz, g_world_text_speaker_screen_coords.vy))
        & 0xF;
    bit = 1;
    best = 0x8000;
    sign = 1;
    for (i = 0; i < 4; i++) {
        if ((s16)(bit & mask) == 0) {
            delta = g_world_camera_candidate_yaws[i] - yaw;
            raw = delta;
            if (delta > 0) {
                wrapped = delta - 0x1000;
            } else {
                wrapped = delta + 0x1000;
            }
            delta = abs(delta);
            wrapped = abs(wrapped);
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
    return g_script_variables[EVENT_SCRIPT_VAR_CAMERA_YAW] + sign * best;
}
