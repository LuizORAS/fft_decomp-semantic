#include "fft/battle_menu.h"
#include "psx/types.h"

/* Same as battle_menu_collect_throwable_items, returning only the low byte (the count); the
 * ability-list builders call it for the Throw menu. */
s32 battle_menu_load_throw_abilities(s32 unit_id, s32 skillset, u8* out_item_ids) {
    return battle_menu_collect_throwable_items(unit_id, skillset & 0xFF, out_item_ids) & 0xFF;
}
