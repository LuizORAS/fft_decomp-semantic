#include "fft/world.h"
#include "psx/libc.h"

/* Filter and reorder 16-bit entries by the selected inventory list.
 *
 * Match each item by its low byte while preserving the full 16-bit input value.
 */
s32 world_item_sort_list_by_category_order(s32 list_index, s16* entries) {
    s16 filtered[140];
    u8* list;
    s32 count;
    s32 i;
    s32 retained;
    s32 j;

    list = g_world_item_sorted_lists[list_index];
    count = 0;
    while (list[count] != ITEM_ID_NONE) {
        count++;
    }
    retained = 0;
    for (i = 0; i < count; i++) {
        for (j = 0; entries[j] != -1; j++) {
            if (list[i] == (u8)entries[j]) {
                filtered[retained] = entries[j];
                retained++;
                break;
            }
        }
    }
    filtered[retained] = -1;
    bcopy(filtered, entries, (retained + 1) * sizeof(s16));
    return retained;
}
