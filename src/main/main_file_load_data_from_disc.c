#include "fft/main.h"

/* Read sectors sectors from sector into destination and wait, polling once per frame, with
 * the Now Loading display that loading_display_mode selects (see main_file_request_read).
 * MAIN loads WLDCORE.BIN and WORLD.BIN this way. */
void main_file_load_data_from_disc(int sector, int sectors, void* destination, int loading_display_mode) {
    main_file_load_descriptor_t* descriptor = &g_main_file_cd_state;

    main_file_request_read(descriptor, sector, sectors, destination, loading_display_mode);
    while (g_main_file_still_loading != 0) {
        main_file_poll_load(descriptor);
        VSync(0);
    }
}
