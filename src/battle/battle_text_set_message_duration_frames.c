#include "fft/battle.h"
#include "psx/types.h"

/* Set how long the next timed battle message (battle_menu_icon_timed_thread) stays up before it closes
 * by itself: `frames` times the event speed, after which it acts as if Circle were pressed; bit 8 also ignores
 * Cancel until then. Callers pass 0x1E (a learned ability), 0x3C (battle messages) or 0xB4 (a level or job
 * level up). */
void battle_text_set_message_duration_frames(u16 frames) {
    g_battle_text_message_duration_frames = frames;
}
