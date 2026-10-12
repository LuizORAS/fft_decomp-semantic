#include "fft/battle.h"
#include "fft/battle_effect.h"

/* Unlinks work from state record record_index, releases its data according
 * to the kind in kind_flags, and returns it to the free chain. */
void effect_free_work_record(s16 record_index, effect_work_record_t* work) {
    effect_record_t* owner;

    owner = &g_effect_state_records[record_index];
    if (work->next != 0) {
        work->next->prev = work->prev;
    }
    if (work->prev != 0) {
        work->prev->next = work->next;
    } else {
        owner->work_head = work->next;
    }
    owner->work_count--;
    switch (work->kind_flags & EFFECT_WORK_KIND_MASK) {
    case EFFECT_WORK_KIND_SPRITE_WORLD:
    case EFFECT_WORK_KIND_SPRITE_SCREEN_OFFSET:
    case EFFECT_WORK_KIND_SPRITE_WORLD_UNSCALED:
    case EFFECT_WORK_KIND_SPRITE_SCREEN:
        effect_free_list_node(work->data);
        break;
    case EFFECT_WORK_KIND_HEAP_BLOCK:
        battle_heap_free_block(work->data);
        break;
    }
    work->next = g_effect_free_work_record_head;
    g_effect_free_work_record_head = work;
    g_effect_work_record_count--;
}
