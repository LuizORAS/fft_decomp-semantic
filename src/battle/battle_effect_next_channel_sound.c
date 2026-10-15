#include "fft/battle.h"

/* Return the next sound of effect sound channel `channel` and count it: the channel's mode picks one of
 * its three ids from the number of sounds it has played (the first, alternating, the first then the second,
 * the first then alternating, or cycling); 0 is no sound. Both timelines' sound tracks call it.
 *
 * Modes above 4 fall through with an unassigned result, as in the target. */
s32 battle_effect_next_channel_sound(s32 channel) {
    s32 count;
    s32 sound;

    count = g_battle_effect_sound_call_counts[channel]++;
    switch (g_battle_effect_flags_section->channels[channel].mode) {
    case EFFECT_SOUND_MODE_FIRST:
        sound = g_battle_effect_flags_section->channels[channel].sound_ids[0];
        break;
    case EFFECT_SOUND_MODE_ALTERNATE:
        sound = g_battle_effect_flags_section->channels[channel].sound_ids[count & 1];
        break;
    case EFFECT_SOUND_MODE_FIRST_THEN_SECOND:
        if (count == 0) {
            sound = g_battle_effect_flags_section->channels[channel].sound_ids[0];
        } else {
            sound = g_battle_effect_flags_section->channels[channel].sound_ids[1];
        }
        break;
    case EFFECT_SOUND_MODE_FIRST_THEN_ALTERNATE:
        if (count != 0) {
            sound = g_battle_effect_flags_section->channels[channel].sound_ids[1 + (count & 1)];
        } else {
            sound = g_battle_effect_flags_section->channels[channel].sound_ids[0];
        }
        break;
    case EFFECT_SOUND_MODE_CYCLE:
        sound = g_battle_effect_flags_section->channels[channel].sound_ids[count % 3];
        break;
    }
    return sound;
}
