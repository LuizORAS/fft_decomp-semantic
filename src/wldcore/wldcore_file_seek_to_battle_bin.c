#include "fft/wldcore.h"

/* Target 0x80068d40. */
void wldcore_file_seek_to_battle_bin(void) {
    wldcore_file_wait_then_build_header(&g_main_file_cd_state, g_main_battle_bin_sector, 1, 0);
}
