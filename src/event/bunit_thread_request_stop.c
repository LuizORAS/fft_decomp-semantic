#include "fft/event_bunit.h"
#include "fft/thread.h"
#include "psx/types.h"

void bunit_thread_request_stop(s32 thread_id) {
    thread_set_parameters(thread_id, 0, 0, 1);
}
