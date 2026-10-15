#include "fft/battle.h"
#include "psx/types.h"

/* Build a scripted walk's path in one call and return it. The function has no return
 * statement: the path is the $v0 that battle_move_calculate_pathing leaves (QUIRKS.md). */
battle_walk_path_t* battle_move_calculate_walkto_pathing(
    s32 flags, s32 jump, s32 x, s32 y, s32 level, s32 target_x, s32 target_y, s32 target_level) {
    s32 suspended;
    battle_move_calculate_pathing(flags, jump, x, y, level, target_x, target_y, target_level, 1, &suspended, 0);
}
