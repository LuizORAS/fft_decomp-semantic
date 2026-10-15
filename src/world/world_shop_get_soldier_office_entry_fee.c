#include "fft/world.h"
#include "psx/types.h"

/* Menu value callback 0 of the office's fee window: the fee of the highlighted row, 1500 for
 * Male fighter and 1400 for Female fighter, 0 otherwise. The fee charged is computed when the recruit
 * is generated (1000 plus its starting equipment's prices), which gives the same two amounts with the
 * retail item prices. */
s32 world_shop_get_soldier_office_entry_fee(void) {
    s16 mode = g_world_shop_hire_menu_thread_data.selected_index;

    if (mode == 0) {
        return 1500;
    }
    return (mode == 1) ? 1400 : 0;
}
