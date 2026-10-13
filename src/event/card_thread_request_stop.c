#include "fft/event_card.h"
#include "fft/thread.h"
#include "psx/types.h"

void card_thread_request_stop(s32 id) {
    thread_set_parameters(id, 0, 0, 1);
}
