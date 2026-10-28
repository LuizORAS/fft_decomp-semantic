#include "fft/main.h"
#include "fft/open.h"

/* Load OPEN.BIN and run the opening and title. Returns 0 when the game goes
 * straight to a battle instead of the world map. */
int main_overlay_exec_open_bin_main_loop(int mode) {
    main_file_load_to_address_checked(DISC_SECTOR_OPEN_OPEN_BIN, DISC_SECTOR_COUNT_OPEN_OPEN_BIN * DISC_SECTOR_BYTES,
        g_main_heap_low_overlay_load_address);
    return open_system_run_main_loop(mode);
}
