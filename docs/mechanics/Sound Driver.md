---
type: mechanic
tier: 4
scope:
  - suzuki_smd
  - suzuki_sound_append_resource()
  - suzuki_smd_set_music_volume()
  - suzuki_smd_stop_music()
  - suzuki_smd_get_active_voice_mask()
  - suzuki_smd_is_music_playing()
  - suzuki_smd_create_music()
  - suzuki_sound_play_direct_sfx()
  - suzuki_sound_play_background_sfx()
  - suzuki_sound_play_sfx()
  - suzuki_sound_set_sfx_volume()
  - suzuki_sound_init()
  - suzuki_sound_stop_all_sfx()
  - suzuki_smd_unload_music()
  - suzuki_sound_init_heap()
  - suzuki_sound_alloc()
  - suzuki_sound_free()
  - suzuki_sound_get_largest_free_block()
  - suzuki_sound_spu_transfer_callback()
---

# Sound Driver

## What it does in the game

The Suzuki driver plays every note and sound effect. It reads SMD sequences
(the music files and the streams of the sound resources) and turns them into
SPU voice commands: which sample, which pitch, how loud, where in the
stereo field, with which envelope, vibrato and reverb. MAIN's game-side
sound code ([[Sound]]) decides what plays and when; the driver plays it.

It runs from an interrupt: every tick of root counter 2, about 240 a second
([[suzuki_sound_init]] has [[SetRCnt]] count to 0x44e8 at the system clock
/ 8),
[[suzuki_sound_root_counter_2_handler]] advances each playing sequence by its
tempo and commits the voice changes, so the music keeps time whatever the
frame rate.

## Data and structures

| Record | What it holds |
|---|---|
| [[suzuki_music_t]] (a MUS record) | One playing sequence: status, tempo and its ramp, master volume, pitch and balance shifts, the bar and play time, voice masks, and its channels. The playing records form the active music list (`g_suzuki_sound_active_music_list`) |
| [[suzuki_music_channel_t]] | One channel of a sequence: its note-data position, octave, length and gate, instrument and envelope, volume, balance and pitch with their ramps, four modulators, loop and repeat stacks, and the SPU voice it owns |
| [[suzuki_smd_header_t]] | The header of an SMD sequence: its channels' offsets, tempo, volume and reverb |
| `g_suzuki_sound_sfx_music` | The SFX record: eight channels on SPU voices 16 to 23 that play the sound effects |
| [[suzuki_sound_resource_t]] | A loaded sound resource ("feds" block); the driver keeps them in a list ([[suzuki_sound_append_resource]]) and plays their streams on the SFX channels |
| `g_suzuki_smd_opcode_handlers`, `g_suzuki_smd_opcode_lengths` | The handlers and parameter lengths of SMD opcodes 0x80 to 0xFF |
| `g_suzuki_smd_note_durations`, `g_suzuki_smd_note_semitones` | A note byte under 0x80 is a semitone (byte / 19) and a length (byte % 19: a whole note is 192 ticks; length 0 reads a length byte) |
| `g_suzuki_smd_pitch_table` | SPU pitch per semitone and fine step at octave 6 ([[suzuki_smd_calculate_pitch]]) |
| [[suzuki_modulator_t]], `g_suzuki_smd_modulator_waveforms` | A channel's four modulators (pitch vibrato, volume tremolo, balance pan LFO, and a fourth only the generic modulator opcodes program) and their waveforms: square, alternating, triangles, sawtooth, sample-and-hold |
| [[suzuki_ramp_t]] | A linear ramp stepped once a tick ([[suzuki_smd_step_ramp]]) |
| The CPU heap | Where MUS records, snapshots, the SFX record and the waveset header live ([[suzuki_sound_alloc]]), apart from MAIN's game heap |

## Main flow

1. **Start.** [[suzuki_sound_init]] starts the driver once: SPU RAM
   allocation, the CPU heap ([[suzuki_sound_init_heap]]), the SFX record, the
   root counter tick, the SPU events and the transfer callback, then the
   sound type, reverb and volumes.
2. **Music.** [[suzuki_smd_create_music]] allocates a MUS record for a loaded SMD
   file, reads its header and channels and queues it;
   MAIN's `main_sound_*` routines then start, fade
   ([[suzuki_smd_set_music_volume]]), pause, resume and stop it
   ([[suzuki_smd_stop_music]], [[suzuki_smd_unload_music]]).
3. **Sound effects.** [[suzuki_sound_play_sfx]] plays a sound of a
   resource on a free pair of SFX channels; [[suzuki_sound_play_direct_sfx]] and
   [[suzuki_sound_play_background_sfx]] use the fixed pairs 6-7 and 4-5.
4. **Every tick**, for each record of the active list:
   [[suzuki_smd_update_channel_ramps]] steps the ramps,
   [[suzuki_smd_read_instructions]] reads each channel's notes and opcodes as
   their lengths run out, [[suzuki_smd_update_modulators]] steps the
   modulators and [[suzuki_smd_update_voices]] works out each voice's pitch,
   volume and balance; the handler then keys the queued voices on and off.
5. **Opcodes.** Each opcode is a handler of `g_suzuki_smd_opcode_handlers`:
   rests and ties, loops and repeats (Loop, End Bar, Repeat, Coda, To
   Coda), octave, tempo and time signature, volume and balance with their
   ramps, instrument and envelope (0xC0-0xCA), pitch offsets and slides
   (0xD0-0xDC), vibrato, tremolo and pan LFO (0xD7-0xEF), the generic
   modulator opcodes (0xF0-0xF7), noise, reverb and pitch LFO, sound
   effects and resources (0x9C-0x9E), the sound font (0xFE) and the end of
   a track (0xFF).
6. **Snapshots.** A record can be saved and restored whole
   ([[suzuki_smd_save_snapshot]], [[suzuki_smd_restore_snapshot]]), and the
   root counter handler restores one when the sequence reaches the record's
   `snapshot_restore_tick`. Nothing in the game saves one:
   [[suzuki_smd_dispatch_snapshot]], the only caller of the save, has no
   caller.

## Where to change

| To change | Edit |
|---|---|
| What a music opcode does | Its handler in `g_suzuki_smd_opcode_handlers` (the `suzuki_smd_*` routine named after it) |
| How notes become pitches | [[suzuki_smd_calculate_pitch]] and `g_suzuki_smd_pitch_table` |
| A modulator waveform | The `suzuki_smd_modulator_step_*` routines |
| Tempo, volume, pitch or balance of a playing track | [[suzuki_smd_set_tempo_scale]], [[suzuki_smd_set_music_volume]], [[suzuki_smd_set_pitch_shift]], [[suzuki_smd_set_balance_shift]] |
| How sound effects take channels | The SFX record and [[suzuki_sound_play_sfx]]; the game-side rules are on the [[Sound]] page |
| The size of the driver's heap | The arena [[suzuki_sound_init]] passes to [[suzuki_sound_init_heap]] |

## Quirks and debts

- Several status and channel flags are tested and never set, or set and
  never tested; [[suzuki_smd_set_flags_all_channels]] and
  [[suzuki_smd_dispatch_snapshot]] have no caller.
- The 37 unassigned opcodes have length 0 in `g_suzuki_smd_opcode_lengths`,
  and 0x9D's length does not match what [[suzuki_smd_stop_sfx]] reads; no
  retail sequence uses them.
- Modulator waveforms 8-15 call the void
  [[suzuki_smd_modulator_deactivate]], and [[suzuki_smd_update_modulators]]
  adds the register it leaves to the channel output.
- [[suzuki_smd_set_instrument]] reads instrument 255, which most music
  channels select before End Bar, past the waveset header kept on the heap.
- [[main_sound_update_tunes]] asks [[suzuki_smd_is_music_playing]] about a null
  record when no scenario music is loaded.
- [[suzuki_sound_init]] reaches code that toggles the root counter event
  before the initialiser has opened it.
- [[suzuki_sound_get_largest_free_block]] reports a gap that includes the next
  block's header.

## Functions in scope

![[Sound Driver scope]]
