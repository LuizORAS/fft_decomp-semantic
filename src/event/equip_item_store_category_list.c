#include "fft/event_equip.h"

/* Compact a -1-terminated s16 item list and append its byte terminator. */
void equip_item_store_category_list(s32 category_index, const s16* source) {
    u8* base;
    s32 count;

    base = g_equip_item_category_lists[category_index];
    count = 0;
    if (*source != -1) {
        do {
            base[count] = *source++;
            count++;
        } while (*source != -1);
    }
    base[count] = EQUIP_ITEM_LIST_END;
}
