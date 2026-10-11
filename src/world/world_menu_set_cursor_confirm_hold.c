#include "fft/world.h"
#include "psx/types.h"

/* While hold is 1 the menu cursors stay in their confirm bob (world_menu_update_entry_cursor_quads,
 * world_menu_update_icon_cursor_sprites); OPEN's music test holds it for ten frames after Circle. */
void world_menu_set_cursor_confirm_hold(s32 hold) {
    g_world_menu_cursor_confirm_hold = hold;
}
