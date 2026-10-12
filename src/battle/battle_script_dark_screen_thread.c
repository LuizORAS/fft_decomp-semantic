#include "fft/battle.h"
#include "fft/event_require.h"
#include "psx/types.h"

/* Thread body of DarkScreen (EVENT_OPCODE_DARK_SCREEN): open REQUIRE.OUT and run its bonus image
 * burst. */
void battle_script_dark_screen_thread(void) {
    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_DARK_SCREEN);
    battle_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_REQUIRE);
    require_reward_animate_bonus_image_burst();
}
