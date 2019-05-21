	.code
	.export	putc
	.align	4
putc:
	sub	$29,$29,16
	stw	$23,$29,0
	add	$23,$0,0xf0300008
	add	$24,$0,0xf030000c
	stw	$24,$29,-4+16
L.2:
L.3:
	ldw	$24,$23,0
	and	$24,$24,1
	beq	$24,$0,L.2
	ldw	$24,$29,-4+16
	add	$15,$0,$4
	stw	$15,$24,0
L.1:
	ldw	$23,$29,0
	add	$29,$29,16
	jr	$31

	.export	main
	.align	4
main:
	sub	$29,$29,32
	stw	$31,$29,16
	add	$4,$0,104
	jal	putc
	add	$4,$0,101
	jal	putc
	add	$4,$0,108
	jal	putc
	add	$4,$0,108
	jal	putc
	add	$4,$0,111
	jal	putc
	add	$2,$0,$0
L.5:
	ldw	$31,$29,16
	add	$29,$29,32
	jr	$31

