---
type: mechanic
tier: 4
scope:
  - effect
---

# Effect Files

## What it does in the game

The disc holds 512 effect files, `EFFECT/E000.BIN` to `E511.BIN`; each
ability with a visual effect names one ([[Ability Effects]] explains how the
file is picked, loaded and played). Most are data only: a script,
timelines, emitters, sprite frames, a palette and sounds that BATTLE's
effect engine plays by itself. **110 of them also carry native code**, and
those are this repository's `effect-*` modules (`target/effect.yaml`). Their
code draws what particles cannot: spinning rings and domes, scrolling
textured grids, ribbons and tentacles, or it changes the battle map
(freezes it, switches its 3D objects, breaks it apart).

The files with code belong to the bigger abilities:
- the high white and time magic (Cure 4, Raise, Raise 2, Reraise, Holy,
  Haste 2, Slow 2, Reflect, Meteor);
- every summon from Shiva to Zodiac;
- the seven dances, two Draw Out swords, Wish and the sword skills Holy
  Explosion, Galaxy Stop and Shock;
- Cloud's eight Limits;
- the Ja spells and the dimension magic;
- Ultima, All-Ultima and many monster and boss skills (Morbol's Tentacle,
  Behemoth, Tiamat, Grand Cross, Midgar Swarm, Snake Carrier...);
- 31 of the effect ids past the abilities (0x201 to 0x225), which no ability
  names: event scripts request them.

Each file's routines say which ability or effect id uses it.

## Data and structures

| Record | What it holds |
|---|---|
| `g_battle_effect_native_callbacks` | The fixed address of each native callback id. A file's code is laid out so that each of its callbacks sits at the address its id names |
| `effect_record_t.callback_phase` ([[effect_phase_e]]) | The phase of a bound callback: INIT allocates its work block, UPDATE runs once a frame, DESTROY frees the block two frames before its keyframe ends |
| [[effect_geometry_entry_t]] and its views | The emitter record a callback reads through its own layout: [[effect_ring_mesh_geometry_view_t]], [[effect_polar_mesh_geometry_view_t]], [[effect_summon_mesh_geometry_view_t]], [[effect_textured_quad_geometry_view_t]], [[effect_particle_ribbon_geometry_view_t]], [[effect_tentacle_mesh_geometry_view_t]] |
| The work blocks of `fft/effect.h` | Each renderer's double-buffered `POLY_GT4` quads and its spin, scroll and wave state, allocated from the effect heap |
| The scratchpad (`0x1f800000`) | The lattice points a mesh renderer builds before it projects them |

## Main flow

1. The effect file loads like any other ([[battle_effect_update_file_load]]).
2. Script opcode 06 puts a native callback in a callback slot
   ([[battle_effect_code_script_06_load_callback_pointer]]); a particle
   keyframe whose action names the slot binds it to the record. Opcode 07
   calls one directly.
3. While that keyframe runs, the particle channel calls the callback once a
   frame instead of the emitter, with the record, the slot, the emitter
   record and the frame. The callback reads its curves (`READ_CURVE`),
   rebuilds its mesh in one buffer, links it into the ordering table and
   flips buffers.
4. Two frames before the keyframe ends the channel sets DESTROY, and the
   callback frees its work block.

| Family | Files | What it draws or does | Example |
|---|---|---|---|
| Ring mesh | 32 | A spinning, flaring ring of 2x32 textured quads | [[effect_e450_update_ring_mesh_state]] |
| Ring stack mesh | 31 | A spinning stack of 8 rings of 8 quads | [[effect_e041_update_ring_stack_mesh_state]] |
| Textured quad grid | 27 (4x4), 7 (8x8), 3 (2x2) | A grid whose texture scrolls along curves | [[effect_e047_update_textured_quad_grid_4x4]] |
| Polar mesh | 23, plus 5 anchored to a target | A four-fold symmetric mesh of quads | [[effect_e461_update_polar_mesh_state]], [[effect_e467_update_anchored_polar_mesh_state]] |
| Summon mesh | 12 | A spinning dome of 8x16 quads | [[effect_e474_update_summon_mesh_state]] |
| Particle ribbons | 8 short, 8 long | Homing particles that trail a ribbon | [[effect_e000_update_particle_ribbons_short_state]] |
| Wave mesh | 8 | An 8x8 grid distorted by waves | [[effect_e033_update_wave_mesh_state]] |
| Tentacle mesh | 6 | Up to eight textured strips that follow points | [[effect_e376_update_tentacle_mesh_state]] |
| Tube mesh | 4 | A spinning closed tube of 9x8 quads | [[effect_e068_update_tube_mesh_state]] |
| Particle trails | 4 | The tentacle renderer without its mesh | [[effect_e077_update_particle_trails_state]] |
| Single ring mesh | 3 | One ring of 32 quads | [[effect_e411_update_single_ring_mesh_state]] |
| Map steps | E450, E453-E456, E458, E480 | Freeze and unfreeze the map, switch its 3D objects, split its mesh, break it, time weather command 0x96 | [[effect_e450_map_step_freeze_state]], [[effect_e456_map_update_specialized_destruction_state]] |
| Particle spawns | E454, E464 | Spawn particles at work vertices: Banish and Altima's transformation | [[effect_e454_particle_update_banish_state]], [[effect_e464_particle_update_altima_transformation_state]] |

## Copies

Each family is one source compiled into every file that uses it, at that
file's addresses. So the 210 routines of the 110 files are 115 distinct
codes: 32 groups are byte for byte identical across files, and a few
differ only in constants. [[effect_add_scratchpad_xyz_components]] is the
one routine that keeps a single name in all 12 files that carry it. A fix
to a family means a fix to every copy.

## Where to change

| To change | Edit |
|---|---|
| What a family draws | Its routine in each file that carries it (see the table) and its record in `fft/effect.h` |
| Which callback an effect uses, and when | The file's script (opcodes 06 and 07) and keyframes: data in the file |
| The curves a renderer follows | The emitter record and the animation curves of the file: data |
| Adding code to an effect file | Waits for the shiftable build: each callback must sit at the address its id names |

## Quirks and debts

- E000's script calls only [[effect_e000_noop]], and E314's names no
  routine of its file, so callback ids 1 and 90 are bound by nothing. Some
  handlers sit in slots no keyframe fires, and E457.BIN, which has no code,
  binds a callback of E259 (see `QUIRKS.md`).
- The `effect_eNNN_update_*` renderers can enter DESTROY before INIT ran;
  the destroy path then reads through a null work slot.
- The wave mesh copies divide by an interpolated value that can be 0.

## Functions in scope

![[Effect Files scope]]
