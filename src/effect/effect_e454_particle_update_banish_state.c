/* E454.BIN is the effect of effect id 0x219, one of the ids past the abilities. */
#include "fft/effect.h"

#define NULL ((void*)0)

/* Banish particle handler: phase 1 allocates the 0x200-byte vertex work and
 * enables map data groups 1-6, phase 2 spawns each active emitter's particles
 * at its work vertices, phase 3 frees the work.
 *
 * The work holds four 16-byte vertex slots per 64-byte row; the overlay table
 * at 0x801c48dc lists (slot, row) byte pairs for emitter groups 1-12, and
 * groups 13 and 14 walk pairs 0-3 and 4-11. */
s32 effect_e454_particle_update_banish_state(s32 record_index, s32 callback_slot, s32 unused_emitter_index, s32 frame) {
    s16 state_index = record_index;
    effect_record_t* record;
    /* VECTOR indexing changes the offset-sum register. */
    u8* work;
    effect_work_record_t parent;
    s32 emitter_index;
    s32 group;
    s32 i;
    s32 offset;
    effect_particle_vertex_emitter_view_t* emitter;
    VECTOR* vertex;
    VECTOR* vertex_work;

    record = &g_battle_effect_state_records[state_index];

    switch (record->callback_phase[callback_slot]) {
    case EFFECT_PHASE_IDLE:
        break;
    case EFFECT_PHASE_INIT:
        if (record->work_slots[callback_slot] != NULL) {
            battle_heap_free_block(record->work_slots[callback_slot]);
        }
        vertex_work = battle_heap_alloc_block(0x200, state_index);
        record->work_slots[callback_slot] = vertex_work;
        g_battle_effect_map_vertices = vertex_work;
        battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_SET_3D_OBJECT_STATE, 1, 1, 1);
        battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_SET_3D_OBJECT_STATE, 2, 1, 1);
        battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_SET_3D_OBJECT_STATE, 3, 1, 1);
        battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_SET_3D_OBJECT_STATE, 4, 1, 1);
        battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_SET_3D_OBJECT_STATE, 5, 1, 1);
        battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_SET_3D_OBJECT_STATE, 6, 1, 1);
        record->callback_phase[callback_slot] = EFFECT_PHASE_UPDATE;
        break;
    case EFFECT_PHASE_UPDATE:
        work = record->work_slots[callback_slot];
        emitter_index = 0;
        do {
            emitter = &g_battle_effect_geometry_table->entries[emitter_index].particle_vertex_emitter;
            group = emitter->vertex_group;
            if (group >= 1 && group <= 12) {
                if (frame >= emitter->start_frame && frame < emitter->end_frame) {
                    i = group - 1;
                    offset = g_effect_e454_particle_vertex_locations[i].slot << 4;
                    offset += g_effect_e454_particle_vertex_locations[i].row << 6;
                    vertex = (VECTOR*)(work + offset);
                    parent.motion.position.x = vertex->vx << 12;
                    parent.motion.position.y = vertex->vy << 12;
                    parent.motion.position.z = vertex->vz << 12;
                    battle_effect_spawn_emitter_particles(
                        record->record_index, record->frame_counter - emitter->start_frame, emitter_index, &parent);
                }
            } else if (group == 13) {
                if (frame >= emitter->start_frame && frame < emitter->end_frame) {
                    for (i = 0; i < 4; i++) {
                        offset = g_effect_e454_particle_vertex_locations[i].slot << 4;
                        offset += g_effect_e454_particle_vertex_locations[i].row << 6;
                        vertex = (VECTOR*)(work + offset);
                        parent.motion.position.x = vertex->vx << 12;
                        parent.motion.position.y = vertex->vy << 12;
                        parent.motion.position.z = vertex->vz << 12;
                        battle_effect_spawn_emitter_particles(
                            record->record_index, record->frame_counter - emitter->start_frame, emitter_index, &parent);
                    }
                }
            } else if (group == 14) {
                if (frame >= emitter->start_frame && frame < emitter->end_frame) {
                    for (i = 4; i < 12; i++) {
                        offset = g_effect_e454_particle_vertex_locations[i].slot << 4;
                        offset += g_effect_e454_particle_vertex_locations[i].row << 6;
                        vertex = (VECTOR*)(work + offset);
                        parent.motion.position.x = vertex->vx << 12;
                        parent.motion.position.y = vertex->vy << 12;
                        parent.motion.position.z = vertex->vz << 12;
                        battle_effect_spawn_emitter_particles(
                            record->record_index, record->frame_counter - emitter->start_frame, emitter_index, &parent);
                    }
                }
            }
            emitter_index++;
        } while (emitter_index < 16);
        break;
    case EFFECT_PHASE_DESTROY:
        if (record->work_slots[callback_slot] != NULL) {
            battle_heap_free_block(record->work_slots[callback_slot]);
            record->work_slots[callback_slot] = NULL;
            g_battle_effect_map_vertices = NULL;
        }
        record->callback_phase[callback_slot] = EFFECT_PHASE_IDLE;
        break;
    }
}
