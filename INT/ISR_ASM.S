; isr_asm.s
			xdef	_vbl_isr
			xdef	_addr_exception_isr
			xdef	_exception_isr
			xdef	_timer_A_isr
			xdef	_ikbd_isr
; kern_asm.s
			xref	_pre_return
; crefs
			xref	_panic
; c isr implementation
			xref	_do_vbl_isr
			xref	_do_addr_exception_isr
			xref	_do_exception_isr
			xref	_do_timer_A_isr
			xref	_do_ikbd_isr

_exception_isr:
	jsr	_do_exception_isr	; doesn't return
	jsr	_panic

addr_exception_isr:
	jsr	_do_addr_exception_isr	; doesn't return
	jsr	_panic

_vbl_isr:
	movem.l	d0-2/a0-2,-(sp)
	jsr	_do_vbl_isr
	movem.l	(sp)+,d0-2/a0-2
	jsr	_pre_return
	rte

_timer_A_isr:
	movem.l	d0-2/a0-2,-(sp)
	jsr	_do_timer_A_isr
	movem.l	(sp)+,d0-2/a0-2
	jsr	_pre_return
	rte

_ikbd_isr:
	movem.l	d0-2/a0-2,-(sp)
	jsr	_do_ikbd_isr
	movem.l	(sp)+,d0-2/a0-2
	jsr	_pre_return
	rte

