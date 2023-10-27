; sys_asm.s
							xdef	_reboot,_sys_reboot
							xdef	_exit,_sys_exit
							xdef	_create_process,_sys_create_process
							xdef	_write,_sys_write
							xdef	_read,_sys_read
							xdef	_get_pid,_sys_get_pid
							xdef	_yield,_sys_yield
; kern_asm.s
			xref	_restart
			xref	_pre_return
; crefs
			xref	_panic
; c syscall implementation
			xref	_do_reboot
			xref	_do_exit
			xref	_do_create_process
			xref	_do_write
			xref	_do_read
			xref	_do_get_pid
			xref	_do_yield

_create_process:
	link	a6,#0
	move.w	10(a6),-(sp)
	move.w	8(a6),-(sp)
	trap	#2
	addq.l	#4,sp
	unlk	a6
	rts

_sys_create_process:
	movem.l	d0-2/a0-2,-(sp)
	move.l	usp,a0
	move.w	2(a0),-(sp)
	move.w	(a0),-(sp)
	jsr	_do_create_process
	addq.l	#4,sp
	movem.l	(sp)+,d0-2/a0-2
	jsr	_pre_return
	rte

_reboot:
	trap	#0		; no parameter

_sys_reboot:
	jsr	_do_reboot	; doesn't return
	jsr	_panic

_exit:
	trap	#1		; no parameter

_sys_exit:
	jsr	_do_exit	; doesn't return
	jsr	_panic

_write:
	link	a6,#0
	move.w	12(a6),-(sp)
	move.l	8(a6),-(sp)
	trap	#3
	unlk	a6
	rts

_sys_write:
	move.l	usp,a0
	move.w	4(a0),-(sp)
	move.l	(a0),-(sp)
	jsr	_do_write
	addq.l	#6,sp
	jsr	_pre_return
	rte

_read:
	link	a6,#0
	move.w	12(a6),-(sp)
	move.l	8(a6),-(sp)
	trap	#4		; returns with output in d0.w
	unlk	a6
	rts

_sys_read:
;movem.l	d0-2/a0-2,-(sp)	; [TO DO] simplify reg save/restore for exceptions & traps
	move.l	usp,a0
	move.w	4(a0),-(sp)
	move.l	(a0),-(sp)
	jsr	_do_read	; returns with output in d0.w
	addq.l	#6,sp
;movem.l	(sp)+,d0-2/a0-2
	jsr	_pre_return
	rte

_get_pid:
	trap	#5		; returns with output in d0.w
	rts

_sys_get_pid:
	jsr	_do_get_pid	; returns with output in d0.w
	jsr	_pre_return
	rte

_yield:
	trap	#7
	rts

_sys_yield:
	jsr	_do_yield	; doesn't return
	jsr	_pre_return
	jsr	_panic

