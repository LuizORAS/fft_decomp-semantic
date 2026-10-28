#include "fft/wldcore.h"

/* Reset a sound-novel record to the start of a reading: no text history, no
 * picture or background, no sound, section 0 and brightness 0x30. */
void wldcore_sound_novel_reset_record(wldcore_sound_novel_record_t* record) {
    record->text_history_3 = -1;
    record->text_history_2 = -1;
    record->text_history_1 = -1;
    record->text_history_0 = -1;
    record->picture_id = -1;
    record->background_set = -1;
    record->sound_mode = 0;
    record->music_selection = 0;
    record->sound_id = 0;
    record->state_flags = 0;
    record->section = 0;
    record->counter = 0;
    record->_unknown_18 = 0;
    record->text_id = 0;
    record->counter_delta = 0;
    record->brightness = 0x30;
}
