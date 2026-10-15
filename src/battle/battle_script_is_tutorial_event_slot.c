#include "fft/battle.h"
#include "psx/types.h"

/* Whether the current event (EVENT_SCRIPT_VAR_CURRENT_EVENT) is one of the sixteen tutorials,
 * EVENT_ID_TUTORIAL_FIRST to EVENT_ID_TUTORIAL_LAST. */
s32 battle_script_is_tutorial_event_slot(void) {
    return (battle_script_get_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT) - EVENT_ID_TUTORIAL_FIRST) < 0x10U;
}
