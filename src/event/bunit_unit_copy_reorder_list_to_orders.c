#include "fft/event_bunit.h"

/* Copy the low byte of each halfword entry into g_main_item_type_order_tables.order_0
 * (the first item type order list at 0x80057b20, 12 bytes, -1 terminated) until the
 * terminator (-1) has been copied. The first parameter is unused. */
void bunit_unit_copy_reorder_list_to_orders(s32 unused, const s16* entries) {
    s32 index = 0;
    s8 value = *entries;

    g_main_item_type_order_tables.order_0[index] = value;
    while (value != -1) {
        entries++;
        value = *entries;
        index += 1;
        g_main_item_type_order_tables.order_0[index] = value;
    }
}
