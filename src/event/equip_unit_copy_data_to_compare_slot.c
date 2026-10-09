#include "fft/event_equip.h"
#include "psx/types.h"

void equip_unit_copy_data_to_compare_slot(s32 unused_selected_index, s32 unused_1) {
    bcopy((u8*)g_equip_unit_data[0], (u8*)g_equip_unit_data[1], 0x7a);
}
