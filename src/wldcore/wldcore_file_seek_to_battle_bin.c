#include "fft/wldcore.h"

/* Request one sector of BATTLE.BIN with no destination, a seek only: the drive moves to
 * BATTLE.BIN while the story event or battle transition plays (wldcore_menu_push_event_transition_level). */
void wldcore_file_seek_to_battle_bin(void) {
    wldcore_file_request_read(&g_main_file_cd_state, g_main_battle_bin_sector, 1, 0);
}
