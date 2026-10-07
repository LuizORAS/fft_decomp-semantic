#include "fft/main.h"

/* Read size bytes from sector into destination and wait, without the Now Loading message:
 * retry the request while the shared descriptor is busy, then poll once per frame. Returns
 * destination. */
void* main_file_load_to_address(int sector, unsigned int size, void* destination) {
    main_file_load_descriptor_t* descriptor;

    while (main_file_request_read_quiet(&g_main_file_cd_state, sector, size >> 11, destination) != 0) {
        VSync(0);
        main_system_frame_hook();
        main_file_poll_load(&g_main_file_cd_state);
    }
    if (g_main_file_still_loading != 0) {
        descriptor = &g_main_file_cd_state;
        do {
            VSync(0);
            main_system_frame_hook();
            main_file_poll_load(descriptor);
        } while (g_main_file_still_loading != 0);
    }
    return destination;
}
