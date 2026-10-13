#include "psx/types.h"

typedef struct world_chained_record {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 length;
    /* 0x02 */ u8 _unused_02[2];
    /* 0x04 */ u8 count_a;
    /* 0x05 */ u8 count_b;
} world_chained_record_t;

/* Return the command after `record` and the commands it counts: an if group (MENU_SCRIPT_OP_IF_GROUP)
 * counts byte 4 children, an if-else group (MENU_SCRIPT_OP_IF_ELSE_GROUP) bytes 4 and 5. The scrollable list
 * uses it to skip a row's commands; the overlays' *_menu_script_skip_group are its twins. */
world_chained_record_t* world_menu_script_skip_group(world_chained_record_t* record) {
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
