#include "fft/main.h"
#include "psx/types.h"

/* Allocate size bytes from the SMD heap and read the sound file at sector into them (whole
 * sectors only), waiting as main_file_load_to_address does. Music and the generic sound effects
 * load this way. A full heap reports MAIN_SYSTEM_ERROR_SMD_LOAD and returns 0; the loader then
 * only seeks, because it skips the read for a null destination. */
void* main_file_alloc_smd_and_load(int sector, u32 size) {
    void* buffer;
    main_file_load_descriptor_t* descriptor;

    buffer = main_heap_alloc_smd(size);
    if (buffer == 0) {
        main_system_handle_error(MAIN_SYSTEM_ERROR_SMD_LOAD);
    }
    while (main_file_request_read_quiet(&g_main_file_cd_state, sector, size >> 11, buffer) != 0) {
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
    return buffer;
}
