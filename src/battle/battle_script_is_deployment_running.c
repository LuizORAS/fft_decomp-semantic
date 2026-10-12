#include "fft/battle.h"
#include "psx/types.h"

/* Run one event frame, then return whether the deployment thread
 * (NATIVE_BORROWED_THREAD_SLOT_DEPLOYMENT) is still running. */
s32 battle_script_is_deployment_running(u32* ot, u32 buttons) {
    battle_script_run_event_frame(ot, buttons);
    return battle_thread_is_running(NATIVE_BORROWED_THREAD_SLOT_DEPLOYMENT);
}
