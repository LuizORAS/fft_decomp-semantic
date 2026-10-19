#include "fft/battle_script.h"
#include "fft/thread.h"
#include "psx/types.h"

/* Run one event frame, then return whether the deployment thread
 * (THREAD_BORROWED_SLOT_DEPLOYMENT) is still running. */
s32 battle_script_is_deployment_running(u32* ot, u32 buttons) {
    battle_script_run_event_frame(ot, buttons);
    return thread_is_running(THREAD_BORROWED_SLOT_DEPLOYMENT);
}
