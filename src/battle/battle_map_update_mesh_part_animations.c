#include "fft/battle.h"
#include "psx/libgte.h"
#include "psx/types.h"

void battle_map_update_mesh_part_animations(void) {
    s32 i;
    u8 next;
    u8 anim;
    s16 duration;
    s16 time;
    SVECTOR* current_b;
    SVECTOR* start_b;

    /* Copying translation through this invariant pointer to part 0, with the
     * destination taken first, matches the target: it hoists
     * &g_battle_map_mesh_parts[0].translation and bases the strength-reduced pointers on
     * it. */
    current_b = &g_battle_map_mesh_parts[0].translation;

    for (i = 1; i < 9; i++) {
        if (g_battle_map_mesh_animation_instructions[i].states[0][1].next != 0) {
            g_battle_map_mesh_parts[i].elapsed
                += (g_battle_map_mesh_parts[i].speed >> 4) * g_main_system_animation_speed;
            if (g_battle_map_mesh_parts[i].duration < g_battle_map_mesh_parts[i].elapsed) {
                next = g_battle_map_mesh_parts[i].next_instruction;
                g_battle_map_mesh_parts[i].keyframe = 0;
                if (g_battle_map_mesh_animation_instructions[i].states[0][next].duration == 0) {
                    g_battle_map_mesh_part_animation_states[i] = 0;
                }
            }
        }
        if (g_battle_map_mesh_part_animation_states[i] != 0) {
            if (g_battle_map_mesh_parts[i].keyframe == 0) {
                g_battle_map_mesh_parts[i].keyframe
                    = g_battle_map_mesh_animation_instructions[i].states[0][next].keyframe;
                anim = g_battle_map_mesh_parts[i].keyframe;
                g_battle_map_mesh_parts[i].next_instruction
                    = g_battle_map_mesh_animation_instructions[i].states[0][next].next;
                g_battle_map_mesh_parts[i].duration
                    = g_battle_map_mesh_animation_instructions[i].states[0][next].duration;
                g_battle_map_mesh_parts[i].rotation_start = g_battle_map_mesh_parts[i].rotation;
                g_battle_map_mesh_parts[i].scale_start = g_battle_map_mesh_parts[i].scale;
                start_b = &g_battle_map_mesh_parts[i].translation_start;
                *start_b = current_b[i * (sizeof(battle_map_mesh_part_t) / sizeof(SVECTOR))];
                g_battle_map_mesh_parts[i].elapsed = 1;
                if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[0] & BATTLE_MAP_MESH_TWEEN_TO) {
                    g_battle_map_mesh_parts[i].rotation_step.vx
                        = g_battle_map_mesh_animation_keyframes[anim].rotation.vx
                        - g_battle_map_mesh_parts[i].rotation_start.vx;
                }
                if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[1] & BATTLE_MAP_MESH_TWEEN_TO) {
                    g_battle_map_mesh_parts[i].rotation_step.vy
                        = g_battle_map_mesh_animation_keyframes[anim].rotation.vy
                        - g_battle_map_mesh_parts[i].rotation_start.vy;
                }
                if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[2] & BATTLE_MAP_MESH_TWEEN_TO) {
                    g_battle_map_mesh_parts[i].rotation_step.vz
                        = g_battle_map_mesh_animation_keyframes[anim].rotation.vz
                        - g_battle_map_mesh_parts[i].rotation_start.vz;
                }
                if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[3] & BATTLE_MAP_MESH_TWEEN_TO) {
                    g_battle_map_mesh_parts[i].translation_step.vx
                        = g_battle_map_mesh_animation_keyframes[anim].translation.vx
                        - g_battle_map_mesh_parts[i].translation_start.vx;
                }
                if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[4] & BATTLE_MAP_MESH_TWEEN_TO) {
                    g_battle_map_mesh_parts[i].translation_step.vy
                        = g_battle_map_mesh_animation_keyframes[anim].translation.vy
                        - g_battle_map_mesh_parts[i].translation_start.vy;
                }
                if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[5] & BATTLE_MAP_MESH_TWEEN_TO) {
                    g_battle_map_mesh_parts[i].translation_step.vz
                        = g_battle_map_mesh_animation_keyframes[anim].translation.vz
                        - g_battle_map_mesh_parts[i].translation_start.vz;
                }
                if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[6] & BATTLE_MAP_MESH_TWEEN_TO) {
                    g_battle_map_mesh_parts[i].scale_step.vx = g_battle_map_mesh_animation_keyframes[anim].scale_x
                        - g_battle_map_mesh_parts[i].scale_start.vx;
                }
                if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[7] & BATTLE_MAP_MESH_TWEEN_TO) {
                    g_battle_map_mesh_parts[i].scale_step.vy = g_battle_map_mesh_animation_keyframes[anim].scale_y
                        - g_battle_map_mesh_parts[i].scale_start.vy;
                }
                if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[8] & BATTLE_MAP_MESH_TWEEN_TO) {
                    g_battle_map_mesh_parts[i].scale_step.vz = g_battle_map_mesh_animation_keyframes[anim].scale_z
                        - g_battle_map_mesh_parts[i].scale_start.vz;
                }
                if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[0] & BATTLE_MAP_MESH_TWEEN_BY) {
                    g_battle_map_mesh_parts[i].rotation_step.vx
                        = g_battle_map_mesh_animation_keyframes[anim].rotation.vx;
                }
                if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[1] & BATTLE_MAP_MESH_TWEEN_BY) {
                    g_battle_map_mesh_parts[i].rotation_step.vy
                        = g_battle_map_mesh_animation_keyframes[anim].rotation.vy;
                }
                if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[2] & BATTLE_MAP_MESH_TWEEN_BY) {
                    g_battle_map_mesh_parts[i].rotation_step.vz
                        = g_battle_map_mesh_animation_keyframes[anim].rotation.vz;
                }
                if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[3] & BATTLE_MAP_MESH_TWEEN_BY) {
                    g_battle_map_mesh_parts[i].translation_step.vx
                        = g_battle_map_mesh_animation_keyframes[anim].translation.vx;
                }
                if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[4] & BATTLE_MAP_MESH_TWEEN_BY) {
                    g_battle_map_mesh_parts[i].translation_step.vy
                        = g_battle_map_mesh_animation_keyframes[anim].translation.vy;
                }
                if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[5] & BATTLE_MAP_MESH_TWEEN_BY) {
                    g_battle_map_mesh_parts[i].translation_step.vz
                        = g_battle_map_mesh_animation_keyframes[anim].translation.vz;
                }
                if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[6] & BATTLE_MAP_MESH_TWEEN_BY) {
                    g_battle_map_mesh_parts[i].scale_step.vx = g_battle_map_mesh_animation_keyframes[anim].scale_x;
                }
                if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[7] & BATTLE_MAP_MESH_TWEEN_BY) {
                    g_battle_map_mesh_parts[i].scale_step.vy = g_battle_map_mesh_animation_keyframes[anim].scale_y;
                }
                if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[8] & BATTLE_MAP_MESH_TWEEN_BY) {
                    g_battle_map_mesh_parts[i].scale_step.vz = g_battle_map_mesh_animation_keyframes[anim].scale_z;
                }
            }
            anim = g_battle_map_mesh_parts[i].keyframe;
            duration = g_battle_map_mesh_parts[i].duration;
            time = g_battle_map_mesh_parts[i].elapsed;
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[0] & BATTLE_MAP_MESH_TWEEN_LINEAR) {
                g_battle_map_mesh_parts[i].rotation.vx = g_battle_map_mesh_parts[i].rotation_step.vx * time / duration;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[1] & BATTLE_MAP_MESH_TWEEN_LINEAR) {
                g_battle_map_mesh_parts[i].rotation.vy = g_battle_map_mesh_parts[i].rotation_step.vy * time / duration;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[2] & BATTLE_MAP_MESH_TWEEN_LINEAR) {
                g_battle_map_mesh_parts[i].rotation.vz = g_battle_map_mesh_parts[i].rotation_step.vz * time / duration;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[3] & BATTLE_MAP_MESH_TWEEN_LINEAR) {
                g_battle_map_mesh_parts[i].translation.vx
                    = g_battle_map_mesh_parts[i].translation_step.vx * time / duration;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[4] & BATTLE_MAP_MESH_TWEEN_LINEAR) {
                g_battle_map_mesh_parts[i].translation.vy
                    = g_battle_map_mesh_parts[i].translation_step.vy * time / duration;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[5] & BATTLE_MAP_MESH_TWEEN_LINEAR) {
                g_battle_map_mesh_parts[i].translation.vz
                    = g_battle_map_mesh_parts[i].translation_step.vz * time / duration;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[6] & BATTLE_MAP_MESH_TWEEN_LINEAR) {
                g_battle_map_mesh_parts[i].scale.vx = g_battle_map_mesh_parts[i].scale_step.vx * time / duration;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[7] & BATTLE_MAP_MESH_TWEEN_LINEAR) {
                g_battle_map_mesh_parts[i].scale.vy = g_battle_map_mesh_parts[i].scale_step.vy * time / duration;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[8] & BATTLE_MAP_MESH_TWEEN_LINEAR) {
                g_battle_map_mesh_parts[i].scale.vz = g_battle_map_mesh_parts[i].scale_step.vz * time / duration;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[0] & BATTLE_MAP_MESH_TWEEN_SINE) {
                g_battle_map_mesh_parts[i].rotation.vx = g_battle_map_mesh_parts[i].rotation_step.vx
                        * rsin((g_battle_map_mesh_animation_keyframes[anim].angle_end[0]
                                   - g_battle_map_mesh_animation_keyframes[anim].angle_start[0])
                                * time / duration
                            + g_battle_map_mesh_animation_keyframes[anim].angle_start[0])
                    >> 12;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[1] & BATTLE_MAP_MESH_TWEEN_SINE) {
                g_battle_map_mesh_parts[i].rotation.vy = g_battle_map_mesh_parts[i].rotation_step.vy
                        * rsin((g_battle_map_mesh_animation_keyframes[anim].angle_end[1]
                                   - g_battle_map_mesh_animation_keyframes[anim].angle_start[1])
                                * time / duration
                            + g_battle_map_mesh_animation_keyframes[anim].angle_start[1])
                    >> 12;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[2] & BATTLE_MAP_MESH_TWEEN_SINE) {
                g_battle_map_mesh_parts[i].rotation.vz = g_battle_map_mesh_parts[i].rotation_step.vz
                        * rsin((g_battle_map_mesh_animation_keyframes[anim].angle_end[2]
                                   - g_battle_map_mesh_animation_keyframes[anim].angle_start[2])
                                * time / duration
                            + g_battle_map_mesh_animation_keyframes[anim].angle_start[2])
                    >> 12;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[3] & BATTLE_MAP_MESH_TWEEN_SINE) {
                g_battle_map_mesh_parts[i].translation.vx = g_battle_map_mesh_parts[i].translation_step.vx
                        * rsin((g_battle_map_mesh_animation_keyframes[anim].angle_end[3]
                                   - g_battle_map_mesh_animation_keyframes[anim].angle_start[3])
                                * time / duration
                            + g_battle_map_mesh_animation_keyframes[anim].angle_start[3])
                    >> 12;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[4] & BATTLE_MAP_MESH_TWEEN_SINE) {
                g_battle_map_mesh_parts[i].translation.vy = g_battle_map_mesh_parts[i].translation_step.vy
                        * rsin((g_battle_map_mesh_animation_keyframes[anim].angle_end[4]
                                   - g_battle_map_mesh_animation_keyframes[anim].angle_start[4])
                                * time / duration
                            + g_battle_map_mesh_animation_keyframes[anim].angle_start[4])
                    >> 12;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[5] & BATTLE_MAP_MESH_TWEEN_SINE) {
                g_battle_map_mesh_parts[i].translation.vz = g_battle_map_mesh_parts[i].translation_step.vz
                        * rsin((g_battle_map_mesh_animation_keyframes[anim].angle_end[5]
                                   - g_battle_map_mesh_animation_keyframes[anim].angle_start[5])
                                * time / duration
                            + g_battle_map_mesh_animation_keyframes[anim].angle_start[5])
                    >> 12;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[6] & BATTLE_MAP_MESH_TWEEN_SINE) {
                g_battle_map_mesh_parts[i].scale.vx = g_battle_map_mesh_parts[i].scale_step.vx
                        * rsin((g_battle_map_mesh_animation_keyframes[anim].angle_end[6]
                                   - g_battle_map_mesh_animation_keyframes[anim].angle_start[6])
                                * time / duration
                            + g_battle_map_mesh_animation_keyframes[anim].angle_start[6])
                    >> 12;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[7] & BATTLE_MAP_MESH_TWEEN_SINE) {
                g_battle_map_mesh_parts[i].scale.vy = g_battle_map_mesh_parts[i].scale_step.vy
                        * rsin((g_battle_map_mesh_animation_keyframes[anim].angle_end[7]
                                   - g_battle_map_mesh_animation_keyframes[anim].angle_start[7])
                                * time / duration
                            + g_battle_map_mesh_animation_keyframes[anim].angle_start[7])
                    >> 12;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[8] & BATTLE_MAP_MESH_TWEEN_SINE) {
                g_battle_map_mesh_parts[i].scale.vz = g_battle_map_mesh_parts[i].scale_step.vz
                        * rsin((g_battle_map_mesh_animation_keyframes[anim].angle_end[8]
                                   - g_battle_map_mesh_animation_keyframes[anim].angle_start[8])
                                * time / duration
                            + g_battle_map_mesh_animation_keyframes[anim].angle_start[8])
                    >> 12;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[0] & BATTLE_MAP_MESH_TWEEN_RAISED_SINE) {
                g_battle_map_mesh_parts[i].rotation.vx = g_battle_map_mesh_parts[i].rotation_step.vx
                        * (rsin((g_battle_map_mesh_animation_keyframes[anim].angle_end[0]
                                    - g_battle_map_mesh_animation_keyframes[anim].angle_start[0])
                                   * time / duration
                               + g_battle_map_mesh_animation_keyframes[anim].angle_start[0])
                            + ONE)
                    >> 12;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[1] & BATTLE_MAP_MESH_TWEEN_RAISED_SINE) {
                g_battle_map_mesh_parts[i].rotation.vy = g_battle_map_mesh_parts[i].rotation_step.vy
                        * (rsin((g_battle_map_mesh_animation_keyframes[anim].angle_end[1]
                                    - g_battle_map_mesh_animation_keyframes[anim].angle_start[1])
                                   * time / duration
                               + g_battle_map_mesh_animation_keyframes[anim].angle_start[1])
                            + ONE)
                    >> 12;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[2] & BATTLE_MAP_MESH_TWEEN_RAISED_SINE) {
                g_battle_map_mesh_parts[i].rotation.vz = g_battle_map_mesh_parts[i].rotation_step.vz
                        * (rsin((g_battle_map_mesh_animation_keyframes[anim].angle_end[2]
                                    - g_battle_map_mesh_animation_keyframes[anim].angle_start[2])
                                   * time / duration
                               + g_battle_map_mesh_animation_keyframes[anim].angle_start[2])
                            + ONE)
                    >> 12;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[3] & BATTLE_MAP_MESH_TWEEN_RAISED_SINE) {
                g_battle_map_mesh_parts[i].translation.vx = g_battle_map_mesh_parts[i].translation_step.vx
                        * (rsin((g_battle_map_mesh_animation_keyframes[anim].angle_end[3]
                                    - g_battle_map_mesh_animation_keyframes[anim].angle_start[3])
                                   * time / duration
                               + g_battle_map_mesh_animation_keyframes[anim].angle_start[3])
                            + ONE)
                    >> 12;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[4] & BATTLE_MAP_MESH_TWEEN_RAISED_SINE) {
                g_battle_map_mesh_parts[i].translation.vy = g_battle_map_mesh_parts[i].translation_step.vy
                        * (rsin((g_battle_map_mesh_animation_keyframes[anim].angle_end[4]
                                    - g_battle_map_mesh_animation_keyframes[anim].angle_start[4])
                                   * time / duration
                               + g_battle_map_mesh_animation_keyframes[anim].angle_start[4])
                            + ONE)
                    >> 12;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[5] & BATTLE_MAP_MESH_TWEEN_RAISED_SINE) {
                g_battle_map_mesh_parts[i].translation.vz = g_battle_map_mesh_parts[i].translation_step.vz
                        * (rsin((g_battle_map_mesh_animation_keyframes[anim].angle_end[5]
                                    - g_battle_map_mesh_animation_keyframes[anim].angle_start[5])
                                   * time / duration
                               + g_battle_map_mesh_animation_keyframes[anim].angle_start[5])
                            + ONE)
                    >> 12;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[6] & BATTLE_MAP_MESH_TWEEN_RAISED_SINE) {
                g_battle_map_mesh_parts[i].scale.vx = g_battle_map_mesh_parts[i].scale_step.vx
                        * (rsin((g_battle_map_mesh_animation_keyframes[anim].angle_end[6]
                                    - g_battle_map_mesh_animation_keyframes[anim].angle_start[6])
                                   * time / duration
                               + g_battle_map_mesh_animation_keyframes[anim].angle_start[6])
                            + ONE)
                    >> 12;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[7] & BATTLE_MAP_MESH_TWEEN_RAISED_SINE) {
                g_battle_map_mesh_parts[i].scale.vy = g_battle_map_mesh_parts[i].scale_step.vy
                        * (rsin((g_battle_map_mesh_animation_keyframes[anim].angle_end[7]
                                    - g_battle_map_mesh_animation_keyframes[anim].angle_start[7])
                                   * time / duration
                               + g_battle_map_mesh_animation_keyframes[anim].angle_start[7])
                            + ONE)
                    >> 12;
            }
            if (g_battle_map_mesh_animation_keyframes[anim].tween_flags[8] & BATTLE_MAP_MESH_TWEEN_RAISED_SINE) {
                g_battle_map_mesh_parts[i].scale.vz = g_battle_map_mesh_parts[i].scale_step.vz
                        * (rsin((g_battle_map_mesh_animation_keyframes[anim].angle_end[8]
                                    - g_battle_map_mesh_animation_keyframes[anim].angle_start[8])
                                   * time / duration
                               + g_battle_map_mesh_animation_keyframes[anim].angle_start[8])
                            + ONE)
                    >> 12;
            }
            g_battle_map_mesh_parts[i].rotation.vx += g_battle_map_mesh_parts[i].rotation_start.vx;
            g_battle_map_mesh_parts[i].rotation.vy += g_battle_map_mesh_parts[i].rotation_start.vy;
            g_battle_map_mesh_parts[i].rotation.vz += g_battle_map_mesh_parts[i].rotation_start.vz;
            g_battle_map_mesh_parts[i].translation.vx += g_battle_map_mesh_parts[i].translation_start.vx;
            g_battle_map_mesh_parts[i].translation.vy += g_battle_map_mesh_parts[i].translation_start.vy;
            g_battle_map_mesh_parts[i].translation.vz += g_battle_map_mesh_parts[i].translation_start.vz;
            g_battle_map_mesh_parts[i].scale.vx += g_battle_map_mesh_parts[i].scale_start.vx;
            g_battle_map_mesh_parts[i].scale.vy += g_battle_map_mesh_parts[i].scale_start.vy;
            g_battle_map_mesh_parts[i].scale.vz += g_battle_map_mesh_parts[i].scale_start.vz;
        }
    }
}
