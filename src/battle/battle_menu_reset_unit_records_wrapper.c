#include "psx/types.h"

/* Call battle_menu_reset_unit_records. Nothing in the game calls it. */
void battle_menu_reset_unit_records_wrapper(void) {
    battle_menu_reset_unit_records();
}
