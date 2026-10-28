#include "fft/main.h"
#include "fft/open.h"

/* Load OPEN.BIN and run the ending cutscene. */
void main_overlay_exec_open_bin_ending(void) {
    main_file_load_to_address_checked(DISC_SECTOR_OPEN_OPEN_BIN, DISC_SECTOR_COUNT_OPEN_OPEN_BIN * DISC_SECTOR_BYTES,
        g_main_heap_low_overlay_load_address);
    open_system_run_ending_cutscene();
}
