#include "fft/event_card.h"
#include "fft/thread.h"
#include "psx/types.h"

/* Ask a thread to stop and run input-update frames until it has ended; then
 * clear the managed-thread state. */
void card_menu_stop_thread_and_wait(s32 id) {
    s32 output;

    while (battle_thread_is_running(id) != 0) {
        card_thread_request_stop(id);
        card_input_update_event_state(&output, 0, 0);
    }
    g_card_thread_state = 0;
}
