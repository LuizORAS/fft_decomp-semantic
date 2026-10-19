#include "fft/menu.h"
#include "fft/world.h"

/* Call menu_reset_unit_records. Nothing in the game calls it, in BATTLE or in WORLD's copy. */
void menu_reset_unit_records_wrapper(void) {
    world_menu_reset_unit_records();
}
