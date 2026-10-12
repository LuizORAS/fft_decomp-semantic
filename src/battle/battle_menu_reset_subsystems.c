#include "fft/battle_event.h"
#include "fft/battle_gfx.h"
#include "fft/battle_menu.h"
#include "fft/battle_thread.h"

/* Reset what the menu layer shares: reseed the event script's random generator,
 * free every window buffer pair, the tpage-7 VRAM grid and the menu buffer arena,
 * and reset the thread scheduler. */
void battle_menu_reset_subsystems(void) {
    battle_script_seed_random_from_vsync();
    battle_menu_reset_buffer_slots();
    battle_gfx_clear_tpage7_vram_allocation_grid();
    battle_menu_init_buffer_allocator();
    battle_thread_reset_scheduler();
}
