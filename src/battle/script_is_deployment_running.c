#include "fft/battle_script.h"
#include "fft/thread.h"
#include "psx/types.h"

/* Run one event frame, then return whether the deployment thread (borrowed slot
 * 6) is still running. */
s32 script_is_deployment_running(u32* ot, u32 buttons) {
    script_run_event_frame(ot, buttons);
    return battle_thread_is_running(NATIVE_BORROWED_THREAD_SLOT_DEPLOYMENT);
}
