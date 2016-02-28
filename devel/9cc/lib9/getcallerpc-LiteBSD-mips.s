LEAF(getcallerpc)
	.set	noreorder
	lw	v0, 28(sp)
	j	ra
	.set reorder
	END(getcallerpc)
