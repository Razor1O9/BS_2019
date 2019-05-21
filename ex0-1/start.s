;
; start.s -- a very simple C runtime startup
;

	.import	main

start:
	add	$29,$0,0xC0010000	; setup stack
	jal	main			; call main()
stop:
	j	stop			; stop by looping
