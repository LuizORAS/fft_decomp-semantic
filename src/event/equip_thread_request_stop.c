#include "fft/event_equip.h"
#include "fft/thread.h"

void equip_thread_request_stop(s32 id) {
    thread_set_parameters(id, 0, 0, 1);
}
