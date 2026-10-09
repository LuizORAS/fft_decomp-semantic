#include "fft/battle.h"
#include "psx/types.h"

void battle_gfx_extract_deployed_unit_palettes(void) {
    RECT rect; /* never used; retail reserves its 8 bytes */

    battle_map_load_palette_data(g_battle_deployed_unit_palettes[0], MAP_PALETTE_BANK_SPRITES, 1, 0);
    battle_map_load_palette_data(g_battle_deployed_unit_palettes[1], MAP_PALETTE_BANK_SPRITES, 2, 0);
    battle_map_load_palette_data(g_battle_gfx_wait_direction_arrow_palette, MAP_PALETTE_BANK_SPRITES, 3, 0);
    battle_map_load_palette_data(g_battle_gfx_map_selection_cursor_palette, MAP_PALETTE_BANK_SPRITES, 4, 0);
    battle_map_load_palette_data(g_battle_gfx_shadow_palette, MAP_PALETTE_BANK_SPRITES, 5, 0);
    battle_map_load_palette_data(g_battle_gfx_status_bubble_palette, MAP_PALETTE_BANK_SPRITES, 7, 0);
}
