#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

#define EVTCHR_BLOCK_SECTORS 15

/*
 * Loads one 0x7800-byte block of EVTCHR.BIN into a fresh heap allocation
 * (the read runs on the main stack), publishing it through g_world_event_pending_loaded_evtchr_slot
 * and g_world_event_loaded_evtchr_buffer before releasing it. Parameter 2
 * selects the block; each occupies 15 sectors from 0x1d4c.
 */
void world_gfx_load_evtchr_thread(void) {
    s32 slot;
    s32 block;
    evtchr_file_record_t* buffer;

    slot = world_thread_get_current_parameter_1();
    block = world_thread_get_current_parameter_2();
    thread_set_current_task_id(THREAD_TASK_LOAD_EVTCHR);
    buffer = main_heap_alloc(sizeof(*buffer));

    do {
        thread_yield();
        g_thread_call_target = (void (*)(void))main_file_call_build_header;
    } while (
        thread_call_on_main_stack(block * EVTCHR_BLOCK_SECTORS + DISC_SECTOR_EVENT_EVTCHR_BIN, sizeof(*buffer), buffer)
        != 0);

    do {
        thread_yield();
        g_thread_call_target = main_file_is_still_loading;
    } while (thread_call_on_main_stack() != 0);

    g_world_event_pending_loaded_evtchr_slot = slot;
    g_world_event_loaded_evtchr_buffer = buffer;
    thread_yield();
    main_heap_free(buffer);
    thread_exit_current();
}
