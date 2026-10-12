#include "fft/battle.h"
#include "psx/types.h"

/* Empty the menu buffer allocator: free all 16 slot records, clear the page
 * occupancy map and release the three typewriter text slots. */
void battle_menu_init_buffer_allocator(void) {
    s32 index;
    u8* allocation_byte;

    index = MENU_BUFFER_ALLOCATION_COUNT - 1;
    do {
        g_battle_menu_allocator_slot_records[index].buffer = (u8*)-1;
        index--;
    } while (index >= 0);

    index = BATTLE_MENU_BUFFER_PAGE_COUNT - 1;
    allocation_byte = &g_battle_menu_buffer_allocation_map[BATTLE_MENU_BUFFER_PAGE_COUNT - 1];
    do {
        *allocation_byte = 0;
        index--;
        allocation_byte--;
    } while (index >= 0);

    index = 0;
    do {
        g_battle_menu_slot_owner_thread_ids[index] = 0;
        g_battle_menu_slot_states[index] = 0;
        index++;
    } while (index < 3);
}
