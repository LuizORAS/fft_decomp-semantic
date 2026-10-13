#ifndef SOUND_H
#define SOUND_H

/* Scenario music loading shared by scene and event overlays. */

#include "psx/types.h"

/* queued effects */
extern s32 g_sound_pending_sfx_id; /* main_sfx_e; -1 means no request */

void sound_queue_sfx_confirm(void);
void sound_queue_sfx_confirm_unless_restoring(void);

/* music */
void sound_play_music_tracks(s32 slot_1_music_id, s32 slot_2_music_id);
void sound_wait_for_music_stub(void);

#endif
