#include "fft/battle.h"
#include "psx/types.h"

/* Striking weapons (WEAPON_FLAG_STRIKING) reach the four adjacent tiles within -6..+5 half levels of
 * the unit's height; lunging ones the tiles 1 and 2 steps away in a straight line, from -8 (-6 at 2
 * steps) to +7 half levels. */
void battle_target_calculate_strike_lunge_range(battle_stats_t* unit, u8 flags) {
    u8 height;
    s32 x;
    s32 y;
    s32 i;
    s32 lo;
    s32 hi;

    height = battle_unit_get_effective_height(unit);
    x = unit->x;
    y = unit->position.bits.y;
    if (flags & WEAPON_FLAG_STRIKING) {
        lo = height - 6;
        hi = height + 5;
        battle_target_mark_tile_in_height_band(x - 1, y, lo, hi);
        battle_target_mark_tile_in_height_band(x + 1, y, lo, hi);
        battle_target_mark_tile_in_height_band(x, y - 1, lo, hi);
        battle_target_mark_tile_in_height_band(x, y + 1, lo, hi);
    } else {
        for (i = 1; i < 3; i++) {
            lo = height + (i - 5) * 2;
            hi = height + 7;
            battle_target_mark_tile_in_height_band(x - i, y, lo, hi);
            battle_target_mark_tile_in_height_band(x + i, y, lo, hi);
            battle_target_mark_tile_in_height_band(x, y - i, lo, hi);
            battle_target_mark_tile_in_height_band(x, y + i, lo, hi);
        }
    }
}
