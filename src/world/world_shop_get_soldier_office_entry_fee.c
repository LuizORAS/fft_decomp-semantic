#include "fft/world.h"
#include "psx/types.h"

/* Menu callback: the hiring fee for the highlighted row of the hire menu,
 * 1500 for row 0, 1400 for row 1 and 0 otherwise. */
s32 world_shop_get_soldier_office_entry_fee(void) {
    s16 mode = g_world_shop_hire_menu_thread_data.selected_index;

    if (mode == 0) {
        return 0x5DC;
    }
    return (mode == 1) ? 0x578 : 0;
}
