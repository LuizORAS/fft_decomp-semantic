#include "fft/world.h"

/* Store sorted item IDs in a byte list.
 *
 * The 16-bit input ends at -1; the byte output ends at 0xff.
 */
void world_item_store_category_list(s32 list_index, s16* sorted_ids) {
    s32 count;
    u8* list;
    u8 item_id;

    list = g_world_item_sorted_lists[list_index];
    for (count = 0; sorted_ids[count] != -1; count++) {
        item_id = sorted_ids[count];
        list[count] = item_id;
    }
    list[count] = ITEM_ID_NONE;
}
