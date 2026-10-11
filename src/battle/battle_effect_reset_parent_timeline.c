#include "psx/types.h"

/* The parent timeline's state in an effect_record_t, as this function clears
 * it: battle_effect_timeline_state_t of opcode 29 lays out the 26 tracks. */
typedef struct battle_effect_parent_timeline_state {
    u8 _unused_00[0x28];
    s16 frame;           /* 0x28 */
    s16 spawn_countdown; /* 0x2a: frames until the next child record starts */
    s16 spawned_count;   /* 0x2c: child records started so far */
    s16 keyframe[26];    /* 0x2e: per track and phase: the keyframe index */
    s16 remaining[26];   /* 0x62: frames left in that keyframe */
    s16 step[26];        /* 0x96: frames it has run */
} battle_effect_parent_timeline_state_t;

void battle_effect_reset_parent_timeline(battle_effect_parent_timeline_state_t* data) {
    s32 index;

    index = 0;
    do {
        data->step[index] = 0;
        data->remaining[index] = 0;
        data->keyframe[index] = 0;
        index++;
    } while (index < 26);
    data->spawn_countdown = 0;
    data->frame = 0;
    data->spawned_count = 0;
}
