;
; sc56.s -- system call test program
;

	.set	SYS_EXIT,1

	.data

	; 56 = not used
sys56:
	add	$8,$0,56
	trap
	add	$8,$0,SYS_EXIT
	add	$4,$0,0
	trap
