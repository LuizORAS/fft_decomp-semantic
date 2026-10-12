---
type: mechanic
tier: 3
scope:
  - battle_script
  - world_script
  - world_tutorial
  - wldcore_script
  - wldcore_sound_novel
  - open_script
  - open_opcode
---

# Event Scripts

## What it does in the game

Story scenes are scripts. Each event is a block of `EVENT/TEST.EVT` that
moves the units and the camera, shows the dialogue, plays music and effects,
changes the map and sets the story's variables. Four smaller script
languages run other parts of the game:

| Script | Run by | What it drives |
|---|---|---|
| Events | BATTLE (`battle_script_*`); WORLD carries a copy (`world_script_*`) | Story scenes, battle starts and ends, Game Over, the battle tutorials |
| Location scripts | WLDCORE (`wldcore_script_*`) | What a world map location does: story events, arrival texts, choices, routes and dots |
| Sound novels | WLDCORE (`wldcore_sound_novel_*`) | The sound novels' reader: text, pictures, choices, sound |
| Formation tutorials | WORLD (`world_tutorial_*`) | The Formation screen's tutorials: buttons pressed for the player, messages, a highlight ring |
| Ending credits | OPEN (`open_script_*`, `open_opcode_*`) | The credits that scroll over the ending's pictures and music |

### Events

- **Picking the event.** At the end of an event, and when the battle asks
  (the scenario check of `battle_menu_dispatch_system_function`), the
  scenario's condition chains pick the next one
  ([[battle_script_run_scenario_conditions]]). ATTACK loads up to ten chains
  per scenario. Each chain is a run of conditions
  ([[event_scenario_condition_e]]: a variable, a unit's presence, HP or MP,
  the acting unit, an equipped item, the war funds, the date, the
  casualties, the enemies defeated, a unit on a tile) that ends in the event
  to run.
- **Running it.** [[battle_script_start_current_event_thread]] starts the
  event thread. It loads the event's 8 KB block
  ([[battle_script_load_event]]) and interprets it
  ([[battle_script_execute_event]]). An instruction is an opcode byte and its
  operands; [[event_opcode_e]] in `include/fft/script.h` names each one with
  its operands. Instructions that take time start a helper thread, and work
  that belongs to the main loop is posted as a request
  ([[battle_script_process_pending_requests]]).
- **A frame** ([[battle_script_run_event_frame]]). Every thread runs once. The
  frame after all of them have stopped, the event ends, and its finish
  operation says what comes next:

  | Result | Next |
  |---|---|
  | 0 | Nothing: the battle goes on |
  | 9 | The world map |
  | 0xA | The next battle |
  | 0xB | A reset |
  | 0x13 | The ending, after *Reunion and Beyond* |

- **Instructions.** The groups:

  | Group | Instructions |
  |---|---|
  | Dialogue | DisplayMessage, ChangeDialog, SetSpeakerPair, ResumeWaitingDialog, FlipMessagePortrait, PortraitCol |
  | Camera | Camera, Focus, FocusSpeed, RelativeCamera, CameraFusionStart and End, CameraSpeedCurve, EarthquakeStart and End |
  | Units | WalkTo, WaitWalk, Jump, FaceUnit, FaceEachOther, RotateUnit, FaceTile, UnitAnim, UnitAnimRotate, SpriteMove, WarpUnit, AddUnit, AddGhostUnit, Draw, Erase, TeleportOut and In, MirrorSprite, ColorUnit, March, InflictStatus, ChangeStats, DismissUnit, BlueRemoveUnit |
  | Map and screen | ChangeMap, MapDarkness, MapLight, Weather, SetDaytime, ColorScreen, ColorField, Background, DarkScreen, Use3DObject, UseFieldObject, SetTileHeight, MapFreeze and Unfreeze |
  | Sound | SoundEffect, SwitchTrack, FadeSound, BgSound, EditBgSound, PlayTune, EndTrack, EndSound |
  | Effects and screens | Effect, EffectStart, EffectEnd, ShowGraphic, DisplayConditions, LoadAttackGraphics |
  | Flow | BlockStart and End, the comparisons, the variable instructions, JumpForward, JumpForwardIfZero, JumpBack and their targets, Wait, WaitValue, Yield, CallFunction ([[event_call_function_e]]), End |

- **Variables.** A script variable id runs from 0 to 0x3ff
  ([[battle_script_get_variable_word_pointer_from_id]]): whole words below
  0x80, one-bit flags from 0x80, nibbles from 0x360. Some words are live
  values: a random number, the camera, the weather and time of day. The
  ranges hold the story progress, the shop level, 32 locals per scenario,
  then the world map's flags (story events, locations, routes, rumors,
  treasures, lands, propositions) and the Brave Story's people
  ([[event_script_variable_id_e]]).
- **Replays.** The Brave Story's replay of a story event and the tutorial
  list set `EVENT_SCRIPT_VAR_SUPPRESS_PROGRESS_EFFECTS`. The event then skips
  DismissUnit, CallFunction, UnlockDate and SetDateAdvance, and
  [[battle_script_run_variable_command]] refuses writes outside the scenario
  locals and the interpreter's working variables.
- **Battle tutorials.** Events 0x19a to 0x1a9, played under tutorial input:
  the script forces and disables buttons
  (`EVENT_SCRIPT_VAR_FORCED_CONTROLLER_INPUTS`, `_DISABLED_`) and the threads
  see that input ([[battle_script_route_event_input]]).
- **WORLD's copy.** [[world_script_execute_event]] runs the same instructions,
  except that WalkTo works only inside a block, Arrow does nothing, DarkScreen
  shows nothing and ChangeMap leaves the light alone.

### Location scripts

Each world map location has a list of scripts
([[wldcore_script_find_location_request]]). A script is a run of conditions
ended by one action, as 16-bit words. The code asks a location for one kind
of action at a time:

| Who asks | For |
|---|---|
| Travel, at the start and at each location reached ([[wldcore_map_start_travel]], [[wldcore_map_step_route_travel]]) | A story event (LoadEvent), else a deferred variable write (SetVar2) |
| The arrival ([[wldcore_menu_restore_level_on_world_entry]]) | The arrival text |
| The location's menu | Its choices, a Deep Dungeon entry |
| The root level ([[wldcore_menu_dispatch_pending_script_requests]]) | A route or a dot to draw or erase, a camera focus |

| Opcodes | What they do |
|---|---|
| 1-4 | Conditions: a variable =, >=, <=, a roster unit with a sprite set |
| 14-19 | Conditions: the war funds, the date, the casualties (>= and <=) |
| 37-40 | Conditions: Ramza's Brave and Faith (>= and <=) |
| 25, 26, 29-36 | Actions: LoadEvent, Choice, Text, Deep Dungeon entry, SetVar2, DrawPath, ErasePath, DrawLocation, EraseLocation, focus |
| 28 | SetVar, written at once whenever the script runs |
| 0, 5-13, 20-24, 27 | Nothing, or operands stepped over |

### Sound novels

WLDCORE reads the sound novels with their own interpreter
([[wldcore_sound_novel_run_until_yield]]), one 32-bit instruction at a time
from 54 opcodes. A novel's script and text come from `SNPLBIN.BIN`
([[wldcore_sound_novel_load_files]]) and its pictures from `WLDPIC.BIN`.
The script shows text and pictures, branches on the choice the reader makes,
at random, on flags or on a counter, plays sounds and music, fades the
screen and runs a countdown. Each section entered is marked read
(`g_main_sound_novel_section_bits`), and the first five novels keep a
bookmark with the save (`g_main_sound_novel_records`).

### Formation tutorials

[[world_tutorial_start]] swaps the player's game for the tutorial's own save,
and [[world_tutorial_step]] plays the tutorial's command stream every frame:
a byte below 0x10 presses that pad button for the player, the others show or
change a message, draw or hide a ring that highlights part of the screen,
wait, or end the tutorial and give the game back.

### Ending credits

OPEN's credits script ([[open_script_run_frame]]) has 16 opcodes: text lines,
the scroll speed, waits (frames or pixels scrolled), the screen and picture
fades, the OPNTEX pictures and slideshows, and the `ENDING.XA` music.

## Data and structures

| Record | What it holds |
|---|---|
| `EVENT/TEST.EVT` ([[event_file_block_t]], `g_main_script_event_block`) | One 8 KB block per event: the instructions, then the event's text, which the load turns into the event message section |
| `g_main_scenario_condition_offsets`, `g_main_scenario_condition_data` | The scenario's condition chains, loaded by ATTACK (`attack_load_scenario_conditionals`) |
| `g_main_scenario_event_finish_operations` | Per event: the next event (bits 0-7 and 10-11) and what follows it |
| `g_main_script_variables` | The script variables: 256 words, the flags and nibbles packed from word 0x80 |
| `g_battle_script_event_instruction_sizes` | Each opcode's operand length; the scanners step with it |
| `g_wldcore_script_base`, [[wldcore_script_state_t]] | The location scripts and the running one's state |
| [[wldcore_sound_novel_record_t]] (`g_wldcore_sound_novel_record`) | The novel being read: section, instruction, text history, picture, sound, counter |
| `g_main_sound_novel_section_bits`, `g_main_sound_novel_saved_flags` | Which sections have been read, and the novels' saved flags |
| [[open_script_state_t]] | The credits script's state, its 16 text lines and the XA steps |

## Main flow

1. **The event starts.** BATTLE enters its event state
   ([[battle_state_enter_event]]) and
   [[battle_script_start_current_event_thread]] starts
   [[battle_script_run_next_event]] on the event thread.
2. **Load and run.** [[battle_script_load_next_event]] waits for the last
   event's instruction threads and loads the block;
   [[battle_script_execute_event]] interprets it.
3. **Each frame.** [[battle_script_run_event_frame]] runs the threads once and
   then [[battle_script_process_pending_requests]].
4. **The end.** EventEnd asks [[battle_script_run_scenario_conditions]] for
   the next event and chains into it, or stops; the frame then returns the
   exit.

## Where to change

| To change | Edit |
|---|---|
| What a scene does | Its block in `TEST.EVT` (instructions and text) |
| Which event follows which | The scenario's condition chains and the finish operations |
| What an instruction does | Its arm of [[battle_script_execute_event]] or its handler, and WORLD's copy |
| What happens at a world map location | Its location scripts |
| A sound novel | Its script and text in `SNPLBIN.BIN`, its pictures in `WLDPIC.BIN` |
| The ending credits | The credits script and its text |

## Quirks and debts

- Instruction values with a length and no handler end the script; a block,
  add-unit or camera-fusion start with no end resumes at byte 1; FocusSpeed
  and RelativeCamera patch the next instruction without checking it; WaitUnitReady
  never waits ([[battle_script_wait_for_unit_ready]]).
- [[battle_script_focus_speed]] adds to an uninitialised register, and
  [[battle_script_toggle_message_portrait_flip]] never sets its record index.
- [[battle_script_get_misc_id]]: the active-turn selector with no unit
  holding the turn reads through NULL.
- Scenario conditions 18 and 19 compare the casualties with the record's id;
  location script opcodes 16 and 17 test the date the other way round from
  scenario conditions 16 and 17.
- CallFunction tests its selectors in sequence, so 0x06 also runs 0x0f's warp.
- [[world_tutorial_command_end]] falls off its end while the fade runs.
- [[wldcore_sound_novel_calculate_completion_percent]] (no caller) counts past
  its array.

## Functions in scope

![[Event Scripts scope]]
