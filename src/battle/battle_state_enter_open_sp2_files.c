#include "fft/battle.h"
#include "psx/types.h"

/* Enter OPEN_SP2_FILES at 60 fps with no SP2 buffer yet. */
void battle_state_enter_open_sp2_files(void) {
    g_battle_state_vsync_interval = 1;
    g_battle_game_state = BATTLE_GAME_STATE_OPEN_SP2_FILES;
    g_battle_gfx_sp2_data = 0;
}
