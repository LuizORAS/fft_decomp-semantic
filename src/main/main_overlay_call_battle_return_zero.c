#include "fft/battle.h"
#include "fft/main.h"

/* Return battle_return_zero() while BATTLE.BIN is loaded, else 0. No C caller. */
int main_overlay_call_battle_return_zero(void) {
    if (g_battle_overlay_loaded == 0) {
        return 0;
    }
    return battle_return_zero();
}
