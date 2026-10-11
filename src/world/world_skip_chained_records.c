#include "fft/world.h"

/* Walks past the records that follow the header, returning the next one. */
world_chained_record_t* world_skip_chained_records(world_chained_record_t* record) {
    s32 count;

    if (record->type == 1) {
        count = record->count_a;
        count += 1;
    } else if (record->type == 2) {
        count = record->count_a;
        count += record->count_b;
        count += 1;
    } else {
        count = 1;
    }
    count -= 1;
    while (count != -1) {
        record = (world_chained_record_t*)((u8*)record + record->length);
        count -= 1;
    }
    return record;
}
