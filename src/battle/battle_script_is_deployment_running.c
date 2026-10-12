#include "fft/battle.h"
#include "fft/battle_event.h"
#include "fft/thread.h"
#include "psx/types.h"

/* Run one event frame, then return whether the deployment thread (borrowed slot
 * 6) is still running. */
s32 battle_script_is_deployment_running(u32* ot, u32 buttons) {
    battle_script_run_event_frame(ot, buttons);
    return battle_thread_is_running(NATIVE_BORROWED_THREAD_SLOT_DEPLOYMENT);
}
