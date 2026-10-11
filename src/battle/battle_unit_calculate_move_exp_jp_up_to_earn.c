#include "fft/battle.h"
#include "psx/types.h"

/* Amount Move-Get EXP or Move-Get JP gives: the number of steps walked
 * (g_battle_move_path[0]), rnd(1..Move) for a teleport and 1 for a failed one,
 * at most 99. */
u8 battle_unit_calculate_move_exp_jp_up_to_earn(battle_stats_t* unit) {
    u8 amount = g_battle_move_path[0];

    if (amount == 0xFE) {
        amount = (rand() * unit->move) / 0x8000 + 1;
    }
    if (amount == 0xFF) {
        amount = 1;
    }
    if (amount >= 0x64) {
        amount = 0x63;
    }
    return amount;
}
