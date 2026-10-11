/* Two commutative operand ties decide this function, in opposite directions.
 * The dispatch block at +0xe4 wants its table address built base-first, which
 * hoisting the action array into its own pointer provides.  The keyframe
 * loop at +0x68 wants the shared `table + 2*k` address built index-first
 * (`addu s0,s0,s2`).  gcc 2.6.3 keeps the source's operand order through to
 * the addu, so `p[i]` and `*(p + i)` build base-first while the int-first
 * sums `*(i + p)` and `i[p]` build index-first -- see the comment at that
 * call.
 */
#include "fft/battle.h"
#include "psx/types.h"

/* Advances one particle channel of the parent timeline by a frame: enters the
 * keyframes that have come due, running each one's action word for every
 * effect target, then spawns the current keyframe's emitter, through the
 * handler of the action's callback slot when it names one. The handler is
 * told to destroy its work on the call made with two frames of the keyframe
 * left. index, remaining and step are the channel's keyframe, the frames left
 * in it and the frames it has run. */
void battle_effect_step_emitter_timeline(battle_keyframe_effect_state_t* state, battle_effect_keyframe_table_t* table,
    s16* index, s16* remaining, s16* step) {
    s16 k;
    s32 sel;
    u16* actions;
    u16 slot;
    s32 emitter;

    if (table->last_index >= *index) {
        while (*remaining == 0) {
            k = *index;
            *index = k + 1;
            /* The int-first sum is load-bearing: the shared address
             * `table + 2*k` is CSEd from this first use, and only this
             * spelling builds it index-first (`addu s0,s0,s2`) as the target
             * does.  `table->action[k + 1]` builds it base-first. */
            battle_effect_run_keyframe_actions_all_targets(*(k + 1 + table->action), state);
            *remaining = table->frame_start[k + 1] - table->frame_start[k];
            *step = 0;
            if (table->last_index < *index) {
                return;
            }
        }
        if (table->last_index >= *index) {
            emitter = table->emitter[*index];
            if (emitter != 0) {
                actions = table->action;
                sel = actions[*index] & EFFECT_KEYFRAME_ACTION_CALLBACK_MASK;
                slot = sel - 1;
                if (sel != 0) {
                    if (*remaining == 2) {
                        state->callback_phase[slot] = EFFECT_PHASE_DESTROY;
                    }
                    state->callbacks[slot](state->record_index, slot, emitter - 1, *step);
                } else {
                    battle_effect_spawn_emitter_particles(state->record_index, *step, emitter - 1, 0);
                }
                *step = *step + 1;
            }
            *remaining = *remaining - 1;
        }
    }
}
