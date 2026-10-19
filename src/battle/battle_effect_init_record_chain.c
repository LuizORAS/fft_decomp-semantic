#include "fft/battle.h"
#include "fft/battle_effect.h"

/* Allocate the 256 effect work records and chain them all into the free list.
 * Also clears the translation of the particle heading and emitter matrices. */
void battle_effect_init_record_chain(void) {
    s32 index;
    battle_effect_work_record_t* records;

    g_battle_effect_particle_heading_matrix.t[2] = 0;
    g_battle_effect_particle_heading_matrix.t[1] = 0;
    g_battle_effect_particle_heading_matrix.t[0] = 0;
    g_battle_effect_emitter_matrix.t[2] = 0;
    g_battle_effect_emitter_matrix.t[1] = 0;
    g_battle_effect_emitter_matrix.t[0] = 0;
    records = battle_heap_alloc_block(EFFECT_WORK_RECORD_CAPACITY * sizeof(battle_effect_work_record_t), 0);
    g_battle_effect_work_records = records;
    g_battle_effect_free_work_record_head = records;
    D_801BC0C4 = 0;
    records[0].prev = 0;
    records[0].next = &records[1];
    for (index = 1; index < EFFECT_WORK_RECORD_CAPACITY - 1; index++) {
        g_battle_effect_work_records[index].next = &g_battle_effect_work_records[index + 1];
        g_battle_effect_work_records[index].prev = &g_battle_effect_work_records[index - 1];
    }
    g_battle_effect_work_record_peak = 0;
    g_battle_effect_work_record_count = 0;
    g_battle_effect_work_records[index].prev = &g_battle_effect_work_records[index - 1];
    g_battle_effect_work_records[index].next = 0;
}
