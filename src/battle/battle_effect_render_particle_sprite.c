#include "fft/battle_camera.h"
#include "fft/battle_effect.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Advances a list node's frame script, rebuilds its sprite-part table when the
 * frame changes, and queues the sprite at the node position.
 *
 * Script bytes: a value below 0x80 is a frame id followed by a hold count and a
 * depth mode; 0x81 restarts the script, 0x82 sets the 16-bit x/y offset and
 * 0x83 adds a signed 8-bit x/y delta. kind bit 0 marks the frame dirty; bits
 * 1-2 select camera projection or a screen-space position, with or without
 * the camera zoom. The oversized frame is reproduced with unused locals. */
void battle_effect_render_particle_sprite(effect_list_node_t* particle, s32 red, s32 green, s32 blue) {
    effect_list_node_t* node;
    SVECTOR input;
    SVECTOR unused_20;
    VECTOR output;
    s32 unused_38[6];
    DVECTOR position;
    battle_effect_sprite_block_t* block;
    battle_effect_sprite_frameset_t* frame;
    s16 frame_table_offset;
    s32 op;
    s16 count;
    s16 i;
    s32 depth;

    node = particle;
    block = node->sprite_block;
    node->frame_timer -= 2;
    if (node->frame_timer <= 0) {
        for (;;) {
            op = node->sequence_data[node->sequence_offset];
            if (!(op & EFFECT_SEQUENCE_OP_FLAG)) {
                if (node->sprite_frame_index != op) {
                    node->sprite_frame_index = op;
                    node->kind |= EFFECT_LIST_NODE_FRAME_CHANGED;
                }
                node->frame_timer = battle_effect_get_current_sequence_instruction(node)->frame.hold;
                node->depth_mode = battle_effect_get_current_sequence_instruction(node)->frame.depth_mode;
                if (node->frame_timer != 0) {
                    node->sequence_offset += 3;
                }
                break;
            }
            switch (op & EFFECT_SEQUENCE_OP_MASK) {
            case EFFECT_SEQUENCE_OP_LOOP:
                node->sequence_offset = 0;
                break;
            case EFFECT_SEQUENCE_OP_SET_OFFSET:
                node->sprite_offset_x = battle_effect_get_current_sequence_instruction(node)->set_offset.x_le[0]
                    + (battle_effect_get_current_sequence_instruction(node)->set_offset.x_le[1] << 8);
                node->sprite_offset_y = battle_effect_get_current_sequence_instruction(node)->set_offset.y_le[0]
                    + (battle_effect_get_current_sequence_instruction(node)->set_offset.y_le[1] << 8);
                node->sequence_offset += 5;
                break;
            case EFFECT_SEQUENCE_OP_ADD_OFFSET:
                node->sprite_offset_x += battle_effect_get_current_sequence_instruction(node)->add_offset.x;
                node->sprite_offset_y += battle_effect_get_current_sequence_instruction(node)->add_offset.y;
                node->sequence_offset += 3;
                break;
            }
        }
    }
    if (node->kind & EFFECT_LIST_NODE_FRAME_CHANGED) {
        frame_table_offset = ((battle_effect_sprite_group_t*)(g_battle_effect_frame_data + node->frame_group_offset))
                                 ->frame_table_offset;
        frame = (battle_effect_sprite_frameset_t*)(*(u16*)(g_battle_effect_frame_data
                                                       + (node->sprite_frame_index * 2 + frame_table_offset))
            + g_battle_effect_frame_data);
        count = frame->count;
        if (block->color.sprite_count != count) {
            battle_effect_free_sprite_block(block);
            block = battle_effect_alloc_sprite_block(count);
            node->sprite_block = block;
        }
        for (i = 0; i < count; i++) {
            block->frames[i] = &frame->parts[i];
        }
        node->kind &= ~EFFECT_LIST_NODE_FRAME_CHANGED;
    }
    block->color.red = red;
    block->color.green = green;
    block->color.blue = blue;
    switch (node->kind & EFFECT_LIST_NODE_PLACEMENT_MASK) {
    case EFFECT_LIST_NODE_WORLD:
        SetRotMatrix(&g_battle_camera_matrix);
        SetTransMatrix(&g_battle_camera_matrix);
        setVector(&input, node->x + node->sprite_offset_x, node->y + node->sprite_offset_y, node->z);
        RotTrans(&input, &output, &output.pad);
        depth = output.vz >> 2;
        switch (node->depth_mode) {
        case EFFECT_SPRITE_DEPTH_NEARER_8:
            depth -= 8;
            break;
        case EFFECT_SPRITE_DEPTH_FIXED_8:
            depth = 8;
            break;
        case EFFECT_SPRITE_DEPTH_FIXED_FARTHEST:
            depth = EFFECT_SPRITE_OT_DEPTH_MAX;
            break;
        case EFFECT_SPRITE_DEPTH_FIXED_16:
            depth = 0x10;
            break;
        case EFFECT_SPRITE_DEPTH_NEARER_16:
            depth -= 0x10;
            break;
        }
        if (depth < 0) {
            return;
        }
        if (depth > EFFECT_SPRITE_OT_DEPTH_MAX) {
            depth = EFFECT_SPRITE_OT_DEPTH_MAX;
        }
        position.vx = output.vx;
        position.vy = output.vy;
        battle_effect_submit_sprite_to_ordering_table(node->sprite_block, &position, node->screen_rotation_angle,
            &g_battle_camera_zoom, g_battle_effect_otag + depth);
        break;
    case EFFECT_LIST_NODE_SCREEN_OFFSET:
        position.vx = node->x + node->sprite_offset_x + g_battle_camera_matrix.t[0];
        position.vy = node->y + node->sprite_offset_y + g_battle_camera_matrix.t[1];
        depth = node->z;
        if (depth < 0) {
            depth = 0;
        }
        if (depth > EFFECT_SPRITE_OT_DEPTH_MAX) {
            depth = EFFECT_SPRITE_OT_DEPTH_MAX;
        }
        battle_effect_submit_sprite_to_ordering_table(node->sprite_block, &position, node->screen_rotation_angle,
            &g_battle_camera_zoom, g_battle_effect_otag + depth);
        break;
    case EFFECT_LIST_NODE_WORLD_UNSCALED:
        SetRotMatrix(&g_battle_camera_matrix);
        SetTransMatrix(&g_battle_camera_matrix);
        setVector(&input, node->x + node->sprite_offset_x, node->y + node->sprite_offset_y, node->z);
        RotTrans(&input, &output, &output.pad);
        depth = output.vz >> 2;
        switch (node->depth_mode) {
        case EFFECT_SPRITE_DEPTH_NEARER_8:
            depth -= 8;
            break;
        case EFFECT_SPRITE_DEPTH_FIXED_8:
            depth = 8;
            break;
        case EFFECT_SPRITE_DEPTH_FIXED_FARTHEST:
            depth = EFFECT_SPRITE_OT_DEPTH_MAX;
            break;
        case EFFECT_SPRITE_DEPTH_FIXED_16:
            depth = 0x10;
            break;
        case EFFECT_SPRITE_DEPTH_NEARER_16:
            depth -= 0x10;
            break;
        }
        if (depth < 0) {
            return;
        }
        if (depth > EFFECT_SPRITE_OT_DEPTH_MAX) {
            depth = EFFECT_SPRITE_OT_DEPTH_MAX;
        }
        position.vx = output.vx;
        position.vy = output.vy;
        battle_effect_submit_sprite_to_ordering_table(
            node->sprite_block, &position, node->screen_rotation_angle, 0, g_battle_effect_otag + depth);
        break;
    case EFFECT_LIST_NODE_SCREEN:
        position.vx = node->x + node->sprite_offset_x;
        position.vy = node->y + node->sprite_offset_y;
        depth = node->z;
        if (depth < 0) {
            depth = 0;
        }
        if (depth > EFFECT_SPRITE_OT_DEPTH_MAX) {
            depth = EFFECT_SPRITE_OT_DEPTH_MAX;
        }
        battle_effect_submit_sprite_to_ordering_table(
            node->sprite_block, &position, node->screen_rotation_angle, 0, g_battle_effect_otag + depth);
        break;
    }
}
