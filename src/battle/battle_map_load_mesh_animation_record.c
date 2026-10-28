#include "fft/battle_map.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Load one record of a map's mesh animation data (GNS_RESOURCE_MESH_ANIMATIONS) and return the
 * address just past it: type 1 the keyframes 1..count (g_battle_map_mesh_animation_keyframes), type 2 the
 * 16 instructions of each of the 8 animation states of mesh parts 1-8
 * (g_battle_map_mesh_animation_instructions), type 3 the parent of each of parts 1..count (4 bytes each,
 * parts past 8 skipped). Type 3 also puts each part at the first keyframe of its running state and marks
 * it animated when that state goes on past its first instruction.
 *
 * The caller (battle_map_dispatch_gns_resource) stops at type 0 before calling, so the result for type 0
 * or an unknown type is never used; `next` is left unset there, which is why the target returns $a1. */
battle_map_mesh_animation_record_t* battle_map_load_mesh_animation_record(battle_map_mesh_animation_record_t* record) {
    battle_map_mesh_animation_record_t* next;
    u32 count;
    u32 i;
    u32 y;
    u32 c;

    count = record->count;
    switch (record->type) {
    case MAP_MESH_ANIMATION_RECORD_KEYFRAMES: {
        battle_map_mesh_keyframe_t* src = (battle_map_mesh_keyframe_t*)record->data;

        count++;
        for (i = 1; i < count; i++) {
            g_battle_map_mesh_animation_keyframes[i] = *src++;
        }
        next = (battle_map_mesh_animation_record_t*)src;
        break;
    }
    case MAP_MESH_ANIMATION_RECORD_END:
        break;
    case MAP_MESH_ANIMATION_RECORD_INSTRUCTIONS: {
        battle_map_mesh_instruction_t* src = (battle_map_mesh_instruction_t*)record->data;

        for (y = 0; y < 8; y++) {
            for (i = 1; i < 9; i++) {
                for (c = 1; c < 17; c++) {
                    g_battle_map_mesh_animation_instructions[i].states[y][c] = *src++;
                }
            }
        }
        next = (battle_map_mesh_animation_record_t*)src;
        break;
    }
    case MAP_MESH_ANIMATION_RECORD_PART_PROPERTIES: {
        u8* src = record->data;

        count++;
        for (i = 1; i < count; i++) {
            if (i >= 9) {
                break;
            }
            g_battle_map_mesh_parts[i].parent = *src;
            g_battle_map_mesh_parts[i].duration = 0;
            g_battle_map_mesh_parts[i].elapsed = 2;
            src += 4;
            if (g_battle_map_mesh_animation_instructions[i].states[0][1].next != 0) {
                g_battle_map_mesh_part_animation_states[i] = 1;
            } else {
                g_battle_map_mesh_part_animation_states[i] = 0;
            }
            g_battle_map_mesh_parts[i].keyframe = 0;
            g_battle_map_mesh_parts[i].rotation
                = g_battle_map_mesh_animation_keyframes
                      [g_battle_map_mesh_animation_instructions[i].states[0][1].keyframe]
                          .rotation;
            g_battle_map_mesh_parts[i].translation
                = g_battle_map_mesh_animation_keyframes
                      [g_battle_map_mesh_animation_instructions[i].states[0][1].keyframe]
                          .translation;
            g_battle_map_mesh_parts[i].scale.vx
                = g_battle_map_mesh_animation_keyframes
                      [g_battle_map_mesh_animation_instructions[i].states[0][1].keyframe]
                          .scale_x;
            g_battle_map_mesh_parts[i].scale.vy
                = g_battle_map_mesh_animation_keyframes
                      [g_battle_map_mesh_animation_instructions[i].states[0][1].keyframe]
                          .scale_y;
            g_battle_map_mesh_parts[i].scale.vz
                = g_battle_map_mesh_animation_keyframes
                      [g_battle_map_mesh_animation_instructions[i].states[0][1].keyframe]
                          .scale_z;
        }
        for (; i < count; i++) {
            src += 4;
        }
        next = (battle_map_mesh_animation_record_t*)src;
        break;
    }
    }
    return next;
}
