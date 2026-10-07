#include "fft/battle.h"

/* Return dividend * 4 / divisor, or 0 for a zero divisor: a share in quarters
 * (battle_ai_init_unit_abilities). */
s32 battle_ai_calculate_ratio_times_4(s32 divisor, s32 dividend) {
    if (divisor == 0) {
        return 0;
    }
    return dividend * 4 / divisor;
}
