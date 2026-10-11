#include "fft/battle.h"
#include "psx/types.h"

/* 1 while the current event id is one of the sixteen tutorial events,
 * 0x19a..0x1a9. */
s32 battle_script_is_tutorial_event_slot(void) {
    return (battle_script_get_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT) - EVENT_ID_TUTORIAL_FIRST) < 0x10U;
}
