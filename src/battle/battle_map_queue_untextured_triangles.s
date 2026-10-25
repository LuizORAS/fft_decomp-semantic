/* Queue the current map part's untextured triangles, culled as
 * battle_map_queue_textured_triangles culls its triangles, as the
 * POLY_F3 at their index in prims, colored
 * g_battle_map_untextured_polygon_color and linked at their average
 * depth (AVSZ3, 0..383) masked with g_battle_map_untextured_otz_mask and
 * combined with g_battle_map_untextured_fixed_ot_index. Assembly for the
 * same reasons: the trapping addi and the loop after its own jr ra.
 *
 * BATTLE.BIN 0x8012d2b4..0x8012d567 (file 0xc62b4..0xc6567).
 */
	.set	noreorder
	.set	noat
	.text
	.align	2
	.globl	battle_map_queue_untextured_triangles
	.ent	battle_map_queue_untextured_triangles
battle_map_queue_untextured_triangles:
	addiu	$sp, $sp, -64
	sw	$s0, 60($sp)
	sw	$s1, 56($sp)
	sw	$s3, 48($sp)
	sw	$s4, 44($sp)
	sw	$s5, 40($sp)
	lui	$s4, %hi(g_battle_map_untextured_otz_mask)
	lw	$s4, %lo(g_battle_map_untextured_otz_mask)($s4)
	lui	$s5, %hi(g_battle_map_untextured_fixed_ot_index)
	lw	$s5, %lo(g_battle_map_untextured_fixed_ot_index)($s5)
	lui	$t0, %hi(D_8012d878)
	addiu	$t0, $t0, %lo(D_8012d878)
	lw	$t1, 0($t0)
	lui	$s1, %hi(g_battle_map_untextured_polygon_color)
	lw	$s1, %lo(g_battle_map_untextured_polygon_color)($s1)
	addi	$t1, $t1, 1
	sw	$t1, 0($t0)
	addu	$s0, $zero, $zero
	addi	$s3, $a2, -1
	andi	$s3, $s3, 0xe00
	srl	$s3, $s3, 0x9
	addi	$s0, $a2, 512
	andi	$s0, $s0, 0x3ff
	andi	$a2, $a2, 0xc00
	srl	$a2, $a2, 0xa
	beq	$s0, $zero, .L8012d324
	nop
	ori	$a2, $zero, 0x4
.L8012d324:
	lui	$t6, %hi(g_battle_map_part_untextured_triangle_count)
	lw	$t6, %lo(g_battle_map_part_untextured_triangle_count)($t6)
	lui	$s0, %hi(g_battle_map_part_untextured_triangle_positions)
	lw	$s0, %lo(g_battle_map_part_untextured_triangle_positions)($s0)
	addi	$a1, $a1, -20
.L8012d338:
	addi	$t6, $t6, -1
	bgez	$t6, .L8012d364
	nop
	lw	$s5, 40($sp)
	lw	$s4, 44($sp)
	lw	$s3, 48($sp)
	lw	$s1, 56($sp)
	lw	$s0, 60($sp)
	addiu	$sp, $sp, 64
	jr	$ra
	nop
.L8012d364:
	lwc2	$0, 0($s0)	# VXY0
	lwc2	$1, 4($s0)	# VZ0
	addi	$s0, $s0, 8
	addi	$a1, $a1, 20
	c2	0x480012	# MVMVA: rotation x V0 + translation
	lh	$a3, 6($s0)
	sb	$s1, 4($a1)
	srl	$t0, $s1, 0x8
	sb	$t0, 5($a1)
	srl	$t0, $s1, 0x10
	sb	$t0, 6($a1)
	mfc2	$t0, $25	# MAC1
	mfc2	$t1, $26	# MAC2
	mfc2	$t9, $27	# MAC3
	lwc2	$0, 0($s0)	# VXY0
	lwc2	$1, 4($s0)	# VZ0
	addi	$s0, $s0, 8
	sll	$t8, $t1, 0x10
	c2	0x480012	# MVMVA: rotation x V0 + translation
	or	$t8, $t0, $t8
	mtc2	$t8, $12	# SXY0
	mtc2	$t9, $17	# SZ1
	sh	$t0, 8($a1)
	sh	$t1, 10($a1)
	mfc2	$t2, $25	# MAC1
	mfc2	$t3, $26	# MAC2
	mfc2	$t9, $27	# MAC3
	lwc2	$0, 0($s0)	# VXY0
	lwc2	$1, 4($s0)	# VZ0
	sll	$t8, $t3, 0x10
	addi	$s0, $s0, 8
	c2	0x480012	# MVMVA: rotation x V0 + translation
	sh	$t2, 12($a1)
	sh	$t3, 14($a1)
	or	$t8, $t2, $t8
	mtc2	$t8, $13	# SXY1
	mtc2	$t9, $18	# SZ2
	mfc2	$t4, $25	# MAC1
	mfc2	$t5, $26	# MAC2
	mfc2	$t9, $27	# MAC3
	sll	$t8, $t5, 0x10
	or	$t8, $t4, $t8
	mtc2	$t8, $14	# SXY2
	mtc2	$t9, $19	# SZ3
	andi	$t7, $a3, 0x3ffc
	srlv	$t8, $t7, $s3
	andi	$t8, $t8, 0x4
	bne	$t8, $zero, .L8012d338
	srlv	$t7, $t7, $a2
	andi	$t7, $t7, 0x400
	bne	$t7, $zero, .L8012d338
	nop
	sh	$t4, 16($a1)
	sh	$t5, 18($a1)
	c2	0x1400006	# NCLIP
	bgez	$t0, .L8012d460
	nop
	bgez	$t2, .L8012d460
	nop
	bgez	$t4, .L8012d460
	nop
	j	.L8012d338
	nop
.L8012d460:
	bgez	$t1, .L8012d480
	nop
	bgez	$t3, .L8012d480
	nop
	bgez	$t5, .L8012d480
	nop
	j	.L8012d338
	nop
.L8012d480:
	addi	$t0, $t0, -640
	bltz	$t0, .L8012d4a4
	addi	$t2, $t2, -640
	bltz	$t2, .L8012d4a4
	addi	$t4, $t4, -640
	bltz	$t4, .L8012d4a4
	nop
	j	.L8012d338
	nop
.L8012d4a4:
	mfc2	$t8, $24	# MAC0
	addi	$t1, $t1, -480
	bltz	$t8, .L8012d338
	addi	$t3, $t3, -480
	bltz	$t1, .L8012d4d4
	addi	$t5, $t5, -480
	bltz	$t3, .L8012d4d4
	nop
	bltz	$t5, .L8012d4d4
	nop
	j	.L8012d338
	nop
.L8012d4d4:
	nop
	nop
	c2	0x158002d	# AVSZ3
	mfc2	$t9, $7	# OTZ
	nop
	bltz	$t9, .L8012d338
	nop
	addu	$t8, $t9, $zero
	addi	$t9, $t9, -384
	bgez	$t9, .L8012d338
	nop
	and	$t8, $t8, $s4
	or	$t8, $t8, $s5
	addu	$t0, $a0, $zero
	sll	$t8, $t8, 0x2
	add	$a0, $a0, $t8
	j	.L8012d524
	nop
	j	.L8012d524
	nop
.L8012d524:
	lui	$t8, 0xff
	ori	$t8, $t8, 0xffff
	lui	$t9, 0xff00
	lw	$v1, 0($a1)
	lw	$v0, 0($a0)
	and	$v1, $v1, $t9
	and	$v0, $v0, $t8
	or	$v1, $v1, $v0
	sw	$v1, 0($a1)
	lw	$v0, 0($a0)
	and	$t1, $a1, $t8
	and	$v0, $v0, $t9
	or	$v0, $v0, $t1
	sw	$v0, 0($a0)
	addu	$a0, $t0, $zero
	j	.L8012d338
	nop
	.end	battle_map_queue_untextured_triangles
	.size	battle_map_queue_untextured_triangles, . - battle_map_queue_untextured_triangles
