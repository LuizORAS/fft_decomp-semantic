#include "fft/world.h"
#include "psx/types.h"

/* Return 1 once the message or dialog thread has drawn its whole text and waits for a confirm
 * (TEXT_END_WAIT_FOR_CONFIRM, or a choice on screen). WLDCORE's message levels close their message on
 * Circle only then. */
s32 world_menu_is_message_waiting_for_confirm(void) {
    return g_world_menu_message_waiting_for_confirm;
}
