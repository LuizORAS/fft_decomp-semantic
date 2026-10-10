#include "fft/world.h"
#include "psx/types.h"

#define EVTCHR_BLOCK_SECTORS 15
#define EVTCHR_BLOCK_BYTES   0x7800

void world_gfx_load_evtchr_thread(void) {
    void* param;
    s32 block;
    void* buffer;

    param = world_thread_get_current_parameter_1();
    block = world_thread_get_current_parameter_2();
    world_thread_set_current_task_id(NATIVE_THREAD_TASK_LOAD_EVTCHR);
    buffer = main_heap_alloc(EVTCHR_BLOCK_BYTES);

    do {
        world_thread_yield();
        g_world_thread_call_target = (void (*)(void))main_file_call_build_header;
    } while (world_thread_call_on_main_stack(
                 block * EVTCHR_BLOCK_SECTORS + DISC_SECTOR_EVENT_EVTCHR_BIN, EVTCHR_BLOCK_BYTES, buffer)
        != 0);

    do {
        world_thread_yield();
        g_world_thread_call_target = main_file_is_still_loading;
    } while (world_thread_call_on_main_stack() != 0);

    g_world_event_pending_loaded_evtchr_slot = param;
    g_world_event_loaded_evtchr_buffer = buffer;
    world_thread_yield();
    main_heap_free(buffer);
    world_thread_exit_current();
}
