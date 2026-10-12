#ifndef BATTLE_SOUND_H
#define BATTLE_SOUND_H

/* Battle music requests, sound effects, and weather audio. */

#include "fft/battle_unit.h"

/* sound */
extern s32 g_battle_sound_primary_weather_sfx_id;
extern s32 g_battle_sound_secondary_weather_sfx_id;
extern s32 g_battle_sound_suppressed;
extern s32 g_battle_sound_tertiary_weather_sfx_id;
extern u8 g_battle_sound_weather_sfx_enabled;
extern s32 g_battle_sound_pending_sfx_id; /* main_sfx_e; -1 while the event frame has no request */
/* main_sfx_e by the weapon's item_type_e (0x20 entries each): the swing, the
 * hit, and the hit a shield or Golem stops (battle_sound_select_weapon_hit). */
extern u8 g_battle_sound_weapon_miss_sfx_ids[];
extern u8 g_battle_sound_weapon_hit_sfx_ids[];
extern u8 g_battle_sound_shield_deflect_sfx_ids[];
extern s16 g_battle_music_set_track_request;

/* SwitchTrack's first operand is a trigger, not the selected track ID.
 * The consumer toggles the zero-based index, then uses main music slot 1/2. */
extern s16 g_battle_music_switch_request; /* 0x80165fc8 */

/* Track IDs are unsigned; -1 in the signed unload request means no request. */
extern u16 g_battle_music_track_1_id;          /* 0x80165fd4 */
extern u16 g_battle_music_track_2_id;          /* 0x80165fd6 */
extern s16 g_battle_music_unload_slot_request; /* 0x80165fca */

/* 0x80165fc0: -1 absent; volume = low 16 bits,
 * duration = (packed >> 14) & 0x3ffc. Not a linear duration field. */
extern s32 g_battle_music_volume_transition_request;
void battle_sound_play_landing_sfx_by_surface_type(battle_unit_misc_data_t* unit);
void battle_sound_play_movement_sfx(battle_unit_misc_data_t* unit, s32 sound_id);
void battle_sound_play_music_tracks(s32 slot_1_music_id, s32 slot_2_music_id);
void battle_sound_select_weapon_hit(battle_unit_misc_data_t* unit, s32 hit);
void battle_sound_set_effect_to_cancel(void);
void battle_sound_set_effect_to_confirm(void);
void battle_sound_set_effect_to_confirm_checked(void);
void battle_sound_set_effect_to_cursor_move(void);
void battle_sound_set_effect_to_invalid(void);
void battle_sound_start_weather_sfx(void);
s32 battle_sound_stop_weather_sfx(void);
void battle_sound_wait_for_music_stub(void);
void battle_sound_bg_thread(void);
void battle_sound_edit_bg_thread(void);

#endif
