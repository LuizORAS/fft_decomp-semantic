#include "psx/libetc.h"

/* Return 1 once 325 horizontal blanks have passed since the last vertical blank wait (VSync(1)). The
 * movement propagations (battle_ai_propagate_target_movement, battle_move_calculate_pathing) suspend
 * there, before the AI's other pauses at 440 and 441. */
s32 battle_ai_is_propagation_budget_spent(void) {
    return VSync(1) >= 0x145;
}
