---
type: mechanic
tier: 3
scope:
  - battle_text
  - world_text
  - world_name
  - text_advance_cursor_with_backreferences()
  - text_advance_cursor_with_separate_backreference_state()
  - text_count_decimal_digits()
  - text_fill_field_with_terminators()
  - text_find_entry()
  - text_measure()
  - text_measure_pixels()
  - text_resolve_format_string_id()
  - text_skip_to_entry()
---

# Text

## What it does in the game

Every line the game shows (dialogue, menus, help, names) is in the game's own
one-byte encoding, and two copies of one text engine read it: BATTLE's
(`battle_text_*`) and WORLD's (`world_text_*`). OPEN and WLDCORE use WORLD's,
which is resident with them. The engine:
- finds a text by its id;
- measures it;
- draws it into a dialog window glyph by glyph, or into a menu's image;
- puts names and numbers in it;
- lets the player type a name.

### Text ids and sections

A text id is 16 bits:
- the top five bits pick one of 32 sections ([[text_section_e]]);
- the low eleven pick the entry (`TEXT_ID_SECTION_MASK`,
  `TEXT_ID_ENTRY_MASK`).

Each engine keeps the 32 section pointers (`g_text_section_pointers`,
`g_text_section_pointers`). An entry is found by counting the
terminators before it ([[text_find_entry]],
[[text_skip_to_entry]]). [[text_id_base_e]] names the first id of each
section the code uses: job names 0x3000, item names 0x3800, unit names 0x4000,
ability names 0x7000, location names 0x9000 and so on.

Screens point sections at their own text while they are up:
- an overlay screen's text goes to section 27 (`TEXT_SECTION_OVERLAY_SCREEN`);
- a list built at run time goes to section 25 (`TEXT_SECTION_COMPOSED_TEXT`);
- the AT list goes to section 31.

A help window binds a whole help file and puts the old table back
afterwards ([[battle_text_save_section_pointers]],
[[battle_text_relocate_pointer_table]],
[[battle_text_restore_section_pointers]]).

The game writes section 8 at run time with the current names: the party's in
WORLD ([[world_text_generate_formation_unit_name_string]]) and the 21 battle
units' in BATTLE ([[battle_text_build_unit_name_list]]). That is how a text
names a unit by its id (control 0xE1, text id 0x4000 + id).

### The text files

Each text file opens with 32 section offsets, counted from the end of that
0x80-byte table. Several files are carried whole inside a module: the bytes
match the file on the disc.

| File | Where it is | What it holds |
|---|---|---|
| EVENT/SAMPLE.LZW | Inside BATTLE.BIN (`g_battle_menu_text_section_offsets`) and WORLD.BIN (`g_world_text_section_offsets`) | Battle text: status and battle messages, job, item, ability, status and skillset names, the menu strings, summon names, and empty room for the unit names |
| EVENT/WORLD.LZW | Loaded into WORLD's text buffer ([[world_menu_init_with_world_text]]) | World map text: names, locations, propositions, rumors, treasures, lands, the Brave Story, the Bar and the shops |
| EVENT/OPEN.LZW | The same buffer for New Game ([[world_menu_init_with_open_text]]) | The unit names, the birthday prompts and the Music Test titles |
| EVENT/HELP.LZW | Inside HELPMENU.OUT, and a second copy inside WORLD.BIN | Battle help: unit status fields, commands, terrain, jobs, items, abilities, skillsets, statuses ([[help_text_section_e]]) |
| EVENT/WLDHELP.LZW | Inside WORLD.BIN (`g_world_text_wldhelp_section_offsets`) | World map help: the party, commands, jobs, items, abilities, places and tips |
| EVENT/JOIN.LZW | Inside REQUIRE.OUT | After-battle warnings and departures ([[require_text_section_e]]) |
| EVENT/ATCHELP.LZW | Loaded by ATTACK.OUT | Deployment help |
| EVENT/SPELL.MES | Loaded into a 16 KB buffer as section 21 | The spell quotes ([[battle_text_run_spell_quote_thread]]) |
| MENU/TUTOn.MES | Loaded as section 23 | The tutorials' messages |
| An event's text | Comes with the event script as section 0 | The event's dialogue |

Despite the `.LZW` names, only HELP.LZW and WLDHELP.LZW are compressed.

### The encoding

Byte values:
- **0x00-0xCF:** a glyph of the font's first page;
- **0xD0-0xDF:** a page prefix: the next byte is a glyph of that page
  (`(prefix & 0xf) * 0xD0 + byte`);
- **0xE0-0xFF:** controls ([[text_control_e]], [[text_format_code_e]]).

| Control | Effect |
|---|---|
| 0xE0, 0xE1, 0xE5, 0xE9, 0xEA, 0xEB | Insert a name: Ramza's, a unit's, a location's, an item's or an ability's by the next substitution value, or any text by id ([[text_resolve_format_string_id]]) |
| 0xE4, 0xE6 | Print the next substitution value, or the one an operand names |
| 0xE8 | The digit count of the next number |
| 0xE2, 0xE3, 0xE7 | The typewriter delay, the palette, the pen's x position |
| 0xEC | The speaker's portrait |
| 0xF0-0xF3 | A back-reference: replay 4 to 35 bytes from up to about 4 KB earlier |
| 0xF4 | A page break: wait for a press |
| 0xF5, 0xF6 | Store a value to a script variable |
| 0xF8 | A new line |
| 0xF9 | Release the threads waiting at 0xFD |
| 0xFA | A 4-pixel space |
| 0xFB, 0xFC | Mark the first choice row, then run the choice |
| 0xFD | Wait until another thread releases this one |
| 0xFE | End, after a press |
| 0xFF | End at once |

A back-reference is three bytes: its length is `(byte 0 & 3) * 8 +
(byte 1 >> 5) + 4`, and it starts `(byte 1 & 0xf) * 254 + byte 2` bytes
before the command. The readers follow it as they go
([[text_advance_cursor_with_backreferences]]), so a compressed file
is never unpacked. The offset is counted in base 254 and bit 4 of byte 1 is
unused. So no byte inside an entry is 0xFE or 0xFF, and the entry scan counts
terminators without decoding anything.

### The dialog thread

A DisplayMessage instruction (see [[Event Scripts]]), a help window or a
spell quote starts the dialog thread ([[battle_text_run_dialog_thread]],
[[world_text_character_handling_thread]]). Its parameters:
1. the dialog type ([[dialog_box_type_e]], [[dialog_options_e]]);
2. the text id;
3. the unit the window's tail points at;
4. the portrait.

Then the thread:
1. measures the text ([[text_measure_pixels]]) and sizes the window
   to it: three lines with a portrait, at most four for Check and Help;
2. draws the glyphs one at a time ([[battle_text_draw_glyph_with_typewriter_delay]]),
   pausing 3 - event speed frames after every 8, 2 or 1 glyphs by the Message
   Speed option (8 while Circle is held);
3. waits at a full window, a page break or the end for Circle, Cross or
   Select. Holding Square there scrolls back through the earlier lines.

The event script steers it through its task:
- **0xFD:** the text waits until a ResumeWaitingDialog instruction names it,
  or another text reaches 0xF9;
- **0xFE in a window with no box:** the thread waits for ChangeDialog, which
  hands it the next text and portrait; 0xFFFF closes it (in WORLD also
  [[world_text_resume_printing]]).

WORLD has a second, simpler message box ([[world_text_message_box_thread]])
for the shops and the name entry.

### The font and the images

- **Font:** each binary carries its own copy of the dialog font
  (`g_battle_text_glyph_bitmaps`, `g_world_text_glyph_bitmaps`): 10 x 14
  glyphs at 2 bits a pixel, 35 bytes each ([[world_text_decode_font_glyph_to_4bpp]]),
  with a width per glyph for proportional text.
- **Menu text:** the menus and overlay screens draw text into 4-bit images
  that they upload to VRAM ([[world_text_render_encoded_ids_to_image]] and
  its overlay copies).
- **Numbers:** they come in three kinds:
  - in the dialog font ([[battle_menu_draw_font_digits]]);
  - in the menu glyph sheet's large digits
    ([[battle_menu_draw_large_number_glyphs]]);
  - in its small digits ([[battle_menu_draw_small_number_glyphs]]).

### The name entry

The name keyboard ([[world_name_open_entry]]) has a row of six special keys
and eight rows of characters, over the message box:
- L1 and R1 move the cursor;
- Square deletes;
- Cross or Start goes to the confirmation.

A name has at most 16 bytes, and an empty entry keeps the original name. Two
places use it:
- **OPEN's New Game:** it asks for Ramza's name with WORLD loaded on OPEN.LZW
  ([[world_name_run_entry_screen]]);
- **The Soldier Office:** the hire and rename steps run it one frame at a time
  ([[world_name_run_entry_keyboard_frame]]).

## Data and structures

| Record | What it holds |
|---|---|
| [[text_control_e]], [[text_format_code_e]] | The control bytes and the name substitutions |
| [[text_id_base_e]], [[text_section_e]], [[help_text_section_e]], [[world_text_section_e]] | Text ids and the sections the code binds |
| [[world_text_backreference_state_t]] | The measuring cursor, kept in the scratchpad |
| [[world_text_dialog_record_t]] | The dialog thread's window |
| [[battle_text_glyph_state_t]] | The glyph waiting to be drawn |
| [[dialog_box_type_e]], [[dialog_options_e]] | The DisplayMessage dialog type's fields |
| `g_text_substitution_values`, `g_text_substitution_values` | The 32 values controls 0xE4 and 0xE6 print |
| [[world_text_data_e]] | WORLD's resident text blocks ([[world_text_bind_section_data]]) |

## Main flow

1. **Bind.** A module or screen points the 32 sections at its text files.
2. **Find.** A text id becomes a pointer to its entry.
3. **Measure.** The width and the lines size the window.
4. **Draw.** The dialog thread or a menu renderer draws it glyph by glyph,
   following back-references and controls.
5. **Wait.** The thread waits for a press, another thread or a new text id,
   then closes the window.

## Where to change

| To change | Edit |
|---|---|
| A line of text | The text file it lives in (the table above); same-size edits only for now, since several files sit inside the binaries |
| The typing speed | [[battle_text_draw_glyph_with_typewriter_delay]] and its WORLD twin |
| How long a timed battle message stays | [[battle_text_set_message_duration_frames]]'s callers |
| A dialog window's size | The sizing at the top of [[battle_text_run_dialog_thread]] and its twin |
| What a control does | Both dialog threads and both measuring routines ([[text_measure]], [[text_measure_pixels]]) |
| A name substitution | [[text_resolve_format_string_id]] and its twin |
| The name keyboard | [[world_name_open_key_rows]], `g_world_name_entry_character_rows`, [[world_name_apply_entry_key]] |
| The font | `g_battle_text_glyph_bitmaps`, `g_world_text_glyph_bitmaps` and the width tables |

## Quirks and debts

- BATTLE's dialog thread prints the window's line count through `FntPrint`
  for each centred line ([[battle_text_print_fnt_value_and_next]]), a debug
  leftover.
- In a centred box, two events keep their lines left-aligned: Meet Velius
  and Searching for Alma.
- WORLD.BIN carries HELP.LZW, which only its own copy of HELPMENU reads.
- Four of the `*_text_render_encoded_ids_to_image` copies (BUNIT, EQUIP,
  JOBSTTS, CARD) never wrap text.
- The six `*_text_render_decimal_value` copies always place a value as one
  digit.
- [[world_text_bind_section_data]] returns its pointer only in `$v0`.

## Functions in scope

![[Text scope]]
