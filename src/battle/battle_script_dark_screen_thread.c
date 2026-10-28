#include "fft/battle_script.h"
#include "fft/event_require.h"
#include "fft/menu.h"
#include "fft/thread.h"

/* Thread body of DarkScreen (EVENT_OPCODE_DARK_SCREEN): open REQUIRE.OUT and run its bonus image
 * burst. */
void battle_script_dark_screen_thread(void) {
    thread_set_current_task_id(THREAD_TASK_DARK_SCREEN);
    menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_REQUIRE);
    require_bonus_run_dark_screen_burst();
}
