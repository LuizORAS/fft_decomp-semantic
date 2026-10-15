#include "fft/battle.h"

/* Choose the best-scored destination at most range from the decision target, or else the nearest
 * tile with no limit (battle_ai_select_destination). */
void battle_ai_select_destination_with_nearest_fallback(s32 range) {
    if (battle_ai_select_destination(range, BATTLE_AI_DESTINATION_PRIORITY_FIRST) == 0) {
        battle_ai_select_destination(0x7fffffff, BATTLE_AI_DESTINATION_DISTANCE_FIRST);
    }
}
