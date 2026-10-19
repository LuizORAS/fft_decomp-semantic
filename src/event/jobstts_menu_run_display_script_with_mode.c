#include "fft/event_jobstts.h"
#include "fft/menu.h"
#include "psx/types.h"

/* Select the palette set for `event_mode` and run the command stream. */
void jobstts_menu_run_display_script_with_mode(menu_instruction_t* script, s32 flags, s32 event_mode) {
    menu_set_palette_mode(event_mode);
    jobstts_menu_run_display_script(script, flags);
}
