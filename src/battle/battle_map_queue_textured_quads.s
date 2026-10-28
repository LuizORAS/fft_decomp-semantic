/* Queue the current map part's textured quads, as
 * battle_map_queue_textured_triangles does its triangles: the average
 * depth of four vertices (AVSZ4), POLY_GT4 packets, and the fourth
 * vertex lit by NCS after the other three by NCT. Assembly for the same
 * reasons: the trapping addi and the loop after its own jr ra.
 *
 * BATTLE.BIN 0x8012cf44..0x8012d2b3 (file 0xc5f44..0xc62b3).
 */
	.set	noreorder
	.set	noat
	.text
	.align	2
	.globl	battle_map_queue_textured_quads
	.ent	battle_map_queue_textured_quads
battle_map_queue_textured_quads:
	addiu	$sp, $sp, -64
	sw	$s0, 60($sp)
	sw	$s1, 56($sp)
	sw	$s2, 52($sp)
	sw	$s3, 48($sp)
	lui	$t0, %hi(D_8012d878)
	addiu	$t0, $t0, %lo(D_8012d878)
	lw	$t1, 0($t0)
	addi	$s3, $a2, -1
	andi	$s3, $s3, 0xe00
	srl	$s3, $s3, 0x9
	addi	$s0, $a2, 512
	andi	$s0, $s0, 0x3ff
	andi	$a2, $a2, 0xc00
	srl	$a2, $a2, 0xa
	beq	$s0, $zero, .L8012cf8c
	nop
	ori	$a2, $zero, 0x4
.L8012cf8c:
	addi	$t1, $t1, 1
	sw	$t1, 0($t0)
	lui	$s2, %hi(g_battle_map_part_textured_quad_normals)
	lw	$s2, %lo(g_battle_map_part_textured_quad_normals)($s2)
	lui	$s1, %hi(g_battle_map_part_textured_quad_positions)
	lw	$s1, %lo(g_battle_map_part_textured_quad_positions)($s1)
	lui	$t6, %hi(g_battle_map_part_textured_quad_count)
	lw	$t6, %lo(g_battle_map_part_textured_quad_count)($t6)
	addi	$a1, $a1, -52
	addi	$s2, $s2, -32
.L8012cfb4:
	addi	$t6, $t6, -1
	bgez	$t6, .L8012cfdc
	nop
	lw	$s3, 48($sp)
	lw	$s2, 52($sp)
	lw	$s1, 56($sp)
	lw	$s0, 60($sp)
	addiu	$sp, $sp, 64
	jr	$ra
	nop
.L8012cfdc:
	lwc2	$0, 0($s1)	# VXY0
	lwc2	$1, 4($s1)	# VZ0
	addi	$s1, $s1, 8
	addi	$a1, $a1, 52
	c2	0x480012	# MVMVA: rotation x V0 + translation
	lh	$a3, 6($s1)
	addi	$s2, $s2, 32
	mfc2	$t0, $25	# MAC1
	mfc2	$t1, $26	# MAC2
	mfc2	$t9, $27	# MAC3
	lwc2	$0, 0($s1)	# VXY0
	lwc2	$1, 4($s1)	# VZ0
	addi	$s1, $s1, 8
	sll	$t8, $t1, 0x10
	c2	0x480012	# MVMVA: rotation x V0 + translation
	sh	$t0, 8($a1)
	sh	$t1, 10($a1)
	or	$t8, $t0, $t8
	mtc2	$t8, $12	# SXY0
	mtc2	$t9, $16	# SZ0
	mfc2	$t2, $25	# MAC1
	mfc2	$t3, $26	# MAC2
	mfc2	$t9, $27	# MAC3
	lwc2	$0, 0($s1)	# VXY0
	lwc2	$1, 4($s1)	# VZ0
	addi	$s1, $s1, 8
	sll	$t8, $t3, 0x10
	c2	0x480012	# MVMVA: rotation x V0 + translation
	sh	$t2, 20($a1)
	sh	$t3, 22($a1)
	or	$t8, $t2, $t8
	mtc2	$t8, $13	# SXY1
	mtc2	$t9, $17	# SZ1
	mfc2	$t4, $25	# MAC1
	mfc2	$t5, $26	# MAC2
	mfc2	$t9, $27	# MAC3
	lwc2	$0, 0($s1)	# VXY0
	lwc2	$1, 4($s1)	# VZ0
	sll	$t8, $t5, 0x10
	addi	$s1, $s1, 8
	c2	0x480012	# MVMVA: rotation x V0 + translation
	sh	$t4, 32($a1)
	sh	$t5, 34($a1)
	or	$t8, $t4, $t8
	mtc2	$t8, $14	# SXY2
	mtc2	$t9, $18	# SZ2
	mfc2	$v0, $25	# MAC1
	mfc2	$v1, $26	# MAC2
	mfc2	$t9, $27	# MAC3
	sll	$t8, $t5, 0x10
	or	$t8, $t4, $t8
	sh	$v0, 44($a1)
	sh	$v1, 46($a1)
	mtc2	$t8, $14	# SXY2
	mtc2	$t9, $19	# SZ3
	andi	$t7, $a3, 0x3ffc
	srlv	$t8, $t7, $s3
	andi	$t8, $t8, 0x4
	bne	$t8, $zero, .L8012cfb4
	srlv	$t7, $t7, $a2
	andi	$t7, $t7, 0x400
	bne	$t7, $zero, .L8012cfb4
	nop
	nop
	c2	0x1400006	# NCLIP
	bgez	$t0, .L8012d108
	nop
	bgez	$t2, .L8012d108
	nop
	bgez	$t4, .L8012d108
	nop
	bgez	$v0, .L8012d108
	nop
	j	.L8012cfb4
	nop
.L8012d108:
	bgez	$t1, .L8012d130
	nop
	bgez	$t3, .L8012d130
	nop
	bgez	$t5, .L8012d130
	nop
	bgez	$v1, .L8012d130
	nop
	j	.L8012cfb4
	nop
.L8012d130:
	addi	$t0, $t0, -640
	bltz	$t0, .L8012d15c
	addi	$t2, $t2, -640
	bltz	$t2, .L8012d15c
	addi	$t4, $t4, -640
	bltz	$t4, .L8012d15c
	addi	$v0, $v0, -640
	bltz	$v0, .L8012d15c
	nop
	j	.L8012cfb4
	nop
.L8012d15c:
	mfc2	$t8, $24	# MAC0
	addi	$t1, $t1, -480
	bltz	$t8, .L8012cfb4
	addi	$t3, $t3, -480
	bltz	$t1, .L8012d194
	addi	$t5, $t5, -480
	bltz	$t3, .L8012d194
	addi	$v1, $v1, -480
	bltz	$t5, .L8012d194
	nop
	bltz	$v1, .L8012d194
	nop
	j	.L8012cfb4
	nop
.L8012d194:
	nop
	nop
	c2	0x168002e	# AVSZ4
	mfc2	$t9, $7	# OTZ
	nop
	bltz	$t9, .L8012cfb4
	nop
	addu	$t8, $t9, $zero
	addi	$t9, $t9, -384
	bgez	$t9, .L8012cfb4
	nop
	addu	$t7, $a3, $zero
	andi	$v1, $t7, 0x1
	beq	$v1, $zero, .L8012d1f0
	nop
	lwc2	$1, 4($s2)	# VZ0
	lwc2	$0, 0($s2)	# VXY0
	lwc2	$2, 8($s2)	# VXY1
	lwc2	$3, 12($s2)	# VZ1
	lwc2	$4, 16($s2)	# VXY2
	lwc2	$5, 20($s2)	# VZ2
	lwc2	$1, 4($s2)	# VZ0
	lwc2	$0, 0($s2)	# VXY0
.L8012d1f0:
	sll	$t8, $t8, 0x2
	addu	$t0, $a0, $zero
	c2	0xd80420	# NCT
	add	$a0, $a0, $t8
	j	.L8012d210
	nop
	j	.L8012d210
	nop
.L8012d210:
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
	andi	$v1, $t7, 0x1
	beq	$v1, $zero, .L8012d2ac
	nop
	mfc2	$t0, $20	# RGB0
	mfc2	$t1, $21	# RGB1
	mfc2	$t2, $22	# RGB2
	lwc2	$0, 24($s2)	# VXY0
	lwc2	$1, 28($s2)	# VZ0
	sh	$t0, 4($a1)
	sh	$t1, 16($a1)
	c2	0xc8041e	# NCS
	sh	$t2, 28($a1)
	srl	$t0, $t0, 0x10
	srl	$t1, $t1, 0x10
	srl	$t2, $t2, 0x10
	sb	$t0, 6($a1)
	sb	$t1, 18($a1)
	mfc2	$t0, $22	# RGB2
	sb	$t2, 30($a1)
	sh	$t0, 40($a1)
	srl	$t0, $t0, 0x10
	sb	$t0, 42($a1)
	j	.L8012d2ac
	nop
.L8012d2ac:
	j	.L8012cfb4
	nop
	.end	battle_map_queue_textured_quads
	.size	battle_map_queue_textured_quads, . - battle_map_queue_textured_quads
