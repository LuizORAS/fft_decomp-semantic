#include "fft/main.h"

/* Return an item's record in g_main_item_primary_data (the id masked to its low byte). */
item_data_t* main_item_get_data_pointer(s32 item_id) {
    return &g_main_item_primary_data[item_id & ITEM_ID_MASK];
}
