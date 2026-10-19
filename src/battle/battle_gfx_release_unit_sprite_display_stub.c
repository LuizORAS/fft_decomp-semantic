#include "fft/battle_gfx.h"

/* Release a removed unit's sprite display; the body is empty. battle_unit_remove passes the
 * display, a member of the unit's g_battle_units record that battle_gfx_init_unit_sprite_display
 * handed out, so there is nothing to free. */
void battle_gfx_release_unit_sprite_display_stub(battle_gfx_sprite_display_data_t* unused_display) {
}
