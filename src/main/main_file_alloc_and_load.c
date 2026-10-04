#include "fft/main.h"
#include "psx/types.h"

/* Allocate size bytes from the game heap and read the file at sector into them, waiting as
 * main_file_load_to_address does. When the heap is full it reports error 1, the null
 * destination makes the read a seek only, and it returns 0. Twin of
 * main_file_alloc_smd_and_load. */
void* main_file_alloc_and_load(s32 sector, s32 size) {
    void* destination;
    main_file_load_descriptor_t* descriptor;

    destination = main_heap_alloc(size);
    if (destination == 0) {
        main_system_handle_animation_exception(1);
    }
    while (main_file_request_read_quiet(&g_main_file_cd_state, sector, (s32)((u32)size >> 11), destination) != 0) {
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
