#include "fft/battle.h"
#include "psx/types.h"

/* Return 1 when a teleport succeeds: always with Teleport 2, else on a roll of 0-99 at or above
 * 10 for each tile of the distance beyond Move, so within Move it always succeeds. */
s32 battle_move_roll_teleport_success(void) {
    battle_move_pathfind_scratch_t* ctx = g_battle_move_config_ptr;
    s32 dy, dx;

    if (ctx->movement_set_2 & BATTLE_MOVEMENT_SET_2_TELEPORT_2) {
        return 1;
    }
    dx = ctx->target_x - ctx->x;
    dy = ctx->target_y - ctx->y;
    if (dx < 0)
        dx = -dx;
    if (dy < 0)
        dy = -dy;
    /* The target passes ctx as a third argument the two-parameter callee ignores. */
    return ((s32 (*)(s32, s32, void*))main_util_roll_pass_fail)(0x64, ((dx + dy) - ctx->move) * 10, ctx);
}

/* padding */
