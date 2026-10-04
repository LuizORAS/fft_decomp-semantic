#include "fft/main.h"
#include "psx/types.h"

/* Allocate size bytes from the SMD heap and read the sound file at sector into them, waiting
 * as main_file_load_to_address does; error 0x11 when that heap is full. Music and the generic
 * sound effects load this way. */
void* main_file_alloc_smd_and_load(int sector, int size) {
    void* buffer;
    main_file_load_descriptor_t* descriptor;

    buffer = main_heap_alloc_smd(size);
    if (buffer == 0) {
        main_system_handle_animation_exception(0x11);
    }
    while (main_file_request_read_quiet(&g_main_file_cd_state, sector, (u32)size >> 11, buffer) != 0) {
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
