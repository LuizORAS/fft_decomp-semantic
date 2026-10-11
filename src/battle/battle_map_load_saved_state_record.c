#include "fft/battle.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Loads one record of the saved map state and returns the address just past it.
 *
 * Type 1 fills entries 1..count of the 0x50-byte table, type 2 the 16 cells of
 * each of 8 rows for entries 1..8 of the 0x220-byte table, and type 3 sets up
 * the 0x98-byte entries 1..8 from the other two tables (4 input bytes per
 * entry, extra entries skipped). The caller at 0x800f32e4 stops at type 0
 * before calling, so the result for type 0 or an unknown type is never used;
 * `next` is left unset there, which is why the target returns $a1. */
battle_map_state_record_t* battle_map_load_saved_state_record(battle_map_state_record_t* record) {
    battle_map_state_record_t* next;
    u32 count;
    u32 i;
    u32 y;
    u32 c;

    count = record->count;
    switch (record->type) {
    case MAP_MESH_ANIMATION_RECORD_KEYFRAMES: {
        u8* src = record->data;

        count++;
        for (i = 1; i < count; i++) {
            g_battle_map_mesh_animation_keyframes[i] = *(battle_map_mesh_keyframe_t*)src;
            src += sizeof(battle_map_mesh_keyframe_t);
        }
        next = (battle_map_state_record_t*)src;
        break;
    }
    case MAP_MESH_ANIMATION_RECORD_END:
        break;
    case MAP_MESH_ANIMATION_RECORD_INSTRUCTIONS: {
        u8* src = record->data;

        for (y = 0; y < 8; y++) {
            for (i = 1; i < 9; i++) {
                for (c = 1; c < 17; c++) {
                    g_battle_map_mesh_animation_instructions[i].states[y][c] = *(battle_map_mesh_instruction_t*)src;
                    src += sizeof(battle_map_mesh_instruction_t);
                }
            }
        }
        next = (battle_map_state_record_t*)src;
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
        next = (battle_map_state_record_t*)src;
        break;
    }
    }
    return next;
}
