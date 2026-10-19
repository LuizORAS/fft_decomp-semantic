#include "fft/event_card.h"
#include "fft/menu.h"

/* Ask thread `thread_id` to stop (the overlay's thread_request_stop: card_thread_request_stop,
 * equip_thread_request_stop) and clear g_menu_managed_thread_state. CARD and EQUIP compile this
 * text. */
void thread_stop_and_clear_state(s32 thread_id) {
    card_thread_request_stop(thread_id);
    g_menu_managed_thread_state = 0;
}
