#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_reset_unit_records_wrapper; nothing in the game calls it. */
void world_menu_reset_unit_records_wrapper(void) {
    world_menu_reset_unit_records();
}
