#include "fft/battle.h"
#include "psx/types.h"

/* Free the high overlay reservation made by battle_menu_reserve_high_overlay. */
void battle_menu_free_high_overlay(void) {
    main_heap_free(g_main_heap_high_overlay_load_address);
}
