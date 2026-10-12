#include "fft/battle_effect.h"
#include "fft/main.h"

/*
 * Take a work record from the free chain and attach it to state record
 * record_index; the inverse of effect_free_work_record.
 *
 * Bits 12-15 of kind select the effect list node type allocated as the
 * record's data (0x0000-0x3000 map to list kinds 0, 2, 4 and 6); other kinds
 * get no data. g_effect_work_record_peak tracks the peak number of records in use.
 */
effect_work_record_t* effect_alloc_work_record(s16 record_index, s16 kind) {
    effect_work_record_t* work;
    effect_record_t* owner;

    work = g_effect_free_work_record_head;
    g_effect_work_record_count++;
    owner = &g_effect_state_records[record_index];
    if (g_effect_work_record_peak < g_effect_work_record_count) {
        g_effect_work_record_peak = g_effect_work_record_count;
    }
    if (work == 0) {
        main_system_report_stub(1, 0x3E);
    }
    g_effect_free_work_record_head = work->next;
    switch (kind & EFFECT_WORK_KIND_MASK) {
    case EFFECT_WORK_KIND_SPRITE_WORLD:
        work->data = effect_alloc_list_node(EFFECT_LIST_NODE_WORLD);
        break;
    case EFFECT_WORK_KIND_SPRITE_SCREEN_OFFSET:
        work->data = effect_alloc_list_node(EFFECT_LIST_NODE_SCREEN_OFFSET);
        break;
    case EFFECT_WORK_KIND_SPRITE_WORLD_UNSCALED:
        work->data = effect_alloc_list_node(EFFECT_LIST_NODE_WORLD_UNSCALED);
        break;
    case EFFECT_WORK_KIND_SPRITE_SCREEN:
        work->data = effect_alloc_list_node(EFFECT_LIST_NODE_SCREEN);
        break;
    }
    work->prev = 0;
    work->next = owner->work_head;
    if (owner->work_head != 0) {
        owner->work_head->prev = work;
    }
    owner->work_head = work;
    owner->work_count++;
    return work;
}
