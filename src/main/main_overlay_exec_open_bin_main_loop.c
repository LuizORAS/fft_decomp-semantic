#include "fft/main.h"

/* Load OPEN.BIN (sector 0x14ff0) and run the opening and title. Returns 0 when the game goes
 * straight to a battle instead of the world map. */
int main_overlay_exec_open_bin_main_loop(int mode) {
    main_file_load_to_address_checked(0x14ff0, 0x36800, g_main_heap_low_overlay_load_address);
    return open_system_run_main_loop(mode);
}
