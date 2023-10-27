; kern_asm.s
					xdef	_restart
; c refs
					xref	_init				; entry
					xref	_kernel_stack_top	; initial SSP 
;OS_ROM_START		equ		$FC0030
;OS_ROM_END			equ		$FF0000

start:
	move.w	#$2700,sr
	reset
	movea.l	_kernel_stack_top,sp	; set up the kernel stack pointer.initial SSP
	jsr		_init					; doesn't normally return (restarts if so)

_restart:
	jmp	start
; note: if we attempt to restart in user mode, a privilege violation will
;       cause the CPU to vector to "_restart" in supervisor mode

