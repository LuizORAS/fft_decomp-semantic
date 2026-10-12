#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Classifies the attack direction and drops evades the target cannot use.
 *
 * The dominant axis of the attacker-to-target offset is compared with the
 * target's facing (position bits 8-11); on a diagonal the target counts as
 * hit from the front when it faces either axis toward the attacker, and a
 * shared tile counts as the back. Side and back attacks clear class evade,
 * back attacks also both shield evades. */
void formula_calculate_facing_evade(void) {
    s32 dx;
    s32 dy;
    u8 distance_x;
    u8 distance_y;
    s32 direction;
    s32 facing;
    u8* result;

    dx = g_action_target->x - g_action_attacker->x;
    distance_x = dx < 0 ? -dx : dx;
    dy = g_action_target->position.y - g_action_attacker->position.y;
    distance_y = dy < 0 ? -dy : dy;
    direction = BATTLE_ABILITY_FACING_SIDE;
    if (distance_y < distance_x) {
        if (dx > 0) {
            facing = g_action_target->position.facing << 8;
            if (facing == BATTLE_UNIT_POSITION_FACING_WEST) {
                direction = BATTLE_ABILITY_FACING_FRONT;
            } else if (facing == BATTLE_UNIT_POSITION_FACING_EAST) {
                direction = BATTLE_ABILITY_FACING_BACK;
            }
        }
        if (dx < 0) {
            facing = g_action_target->position.facing << 8;
            if (facing == BATTLE_UNIT_POSITION_FACING_EAST) {
                direction = BATTLE_ABILITY_FACING_FRONT;
            } else if (facing == BATTLE_UNIT_POSITION_FACING_WEST) {
                direction = BATTLE_ABILITY_FACING_BACK;
            }
        }
    } else if (distance_x < distance_y) {
        if (dy > 0) {
            facing = g_action_target->position.facing << 8;
            if (facing == BATTLE_UNIT_POSITION_FACING_SOUTH) {
                direction = BATTLE_ABILITY_FACING_FRONT;
            } else if (facing == BATTLE_UNIT_POSITION_FACING_NORTH) {
                direction = BATTLE_ABILITY_FACING_BACK;
            }
        }
        if (dy < 0) {
            facing = g_action_target->position.facing << 8;
            if (facing == BATTLE_UNIT_POSITION_FACING_NORTH) {
                direction = BATTLE_ABILITY_FACING_FRONT;
            } else if (facing == BATTLE_UNIT_POSITION_FACING_SOUTH) {
                direction = BATTLE_ABILITY_FACING_BACK;
            }
        }
    } else if (dx > 0 && (g_action_target->position.facing << 8) == BATTLE_UNIT_POSITION_FACING_WEST) {
        direction = BATTLE_ABILITY_FACING_FRONT;
    } else if (dx < 0 && (g_action_target->position.facing << 8) == BATTLE_UNIT_POSITION_FACING_EAST) {
        direction = BATTLE_ABILITY_FACING_FRONT;
    } else if (dy > 0 && (g_action_target->position.facing << 8) == BATTLE_UNIT_POSITION_FACING_SOUTH) {
        direction = BATTLE_ABILITY_FACING_FRONT;
    } else if (dy < 0 && (g_action_target->position.facing << 8) == BATTLE_UNIT_POSITION_FACING_NORTH) {
        direction = BATTLE_ABILITY_FACING_FRONT;
    } else if (dx == 0 && dy == 0) {
        direction = BATTLE_ABILITY_FACING_BACK;
    }
    result = &g_action_current_ability.facing_modifier;
    *result = direction;
    if (*result != BATTLE_ABILITY_FACING_FRONT) {
        g_action_current_ability.class_evade = 0;
    }
    if (*result == BATTLE_ABILITY_FACING_BACK) {
        g_action_current_ability.right_hand_evade = 0;
        g_action_current_ability.left_hand_evade = 0;
    }
}
