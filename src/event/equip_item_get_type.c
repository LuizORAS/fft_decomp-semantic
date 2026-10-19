#include "fft/event_equip.h"

/* An item's type (item_type_e) in the item table. */
s32 equip_item_get_type(s32 item_id) {
    return g_main_item_primary_data[item_id & EQUIP_ITEM_ID_MASK].type;
}
