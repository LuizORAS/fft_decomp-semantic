#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* Runs one WORLD frame and returns whether WORLD thread 6 is still running. */
s32 world_script_is_deployment_running(u32* ot, u32 buttons) {
    world_script_run_frame(ot, buttons);
    return thread_is_running(THREAD_BORROWED_SLOT_DEPLOYMENT);
}
