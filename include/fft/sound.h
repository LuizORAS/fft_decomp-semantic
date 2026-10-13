#ifndef SOUND_H
#define SOUND_H

/* Scenario music loading shared by scene and event overlays. */

#include "psx/types.h"

/* music */
void sound_play_music_tracks(s32 slot_1_music_id, s32 slot_2_music_id);
void sound_wait_for_music_stub(void);

#endif
