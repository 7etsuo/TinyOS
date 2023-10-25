					xdef	_restart
					xdef	_load_cpu_context
				
					xdef	_exit,_sys_exit
					xref	_do_exit
					
					xdef	_create_process,_sys_create_process
					xref	_do_create_process
					
					xdef	_write,_sys_write
					xref	_do_write
					
					xdef	_read,_sys_read
					xref	_do_read

					xdef	_get_pid,_sys_get_pid
					xref	_do_get_pid
					
					xdef	_yield,_sys_yield
					xref	_do_yield
					
					xdef	_vbl_isr
					xref	_do_vbl_isr
					xdef	_addr_exception_isr
					xref	_do_addr_exception_isr
					xdef	_exception_isr
					xref	_do_exception_isr
					xref	_init
					xdef	_read_SR,_write_SR
					xdef	_await_interrupt
					xdef	_timer_A_isr
					xref	_do_timer_A_isr
					xdef	_ikbd_isr
					xref	_do_ikbd_isr
					xref	_proc,_curr_proc
					xdef	_clear_screen
					xdef	_scroll

					xref	_resched_needed
					xref	_schedule
					xref	_panic

;OS_ROM_START		equ		$FC0030
;OS_ROM_END			equ		$FF0000

OS_RAM_TOP			equ		$000800					; initial SSP

VIDEO_BASE			equ		$3F8000

CPU_CONTEXT_SIZE	equ		70
PROCESS_ENTRY_SIZE	equ		102

start:				move.w	#$2700,sr
					reset
					movea.l	#OS_RAM_TOP,sp
					jsr		_init					; doesn't normally return (restarts if so)

_restart:			jmp		start

					; note: if we attempt to restart in user mode, a privilege violation will
					;       cause the CPU to vector to "_restart" in supervisor mode


_exception_isr:		jsr		_do_exception_isr		; doesn't return
					jsr		_panic

_addr_exception_isr:
					jsr		_do_addr_exception_isr	; doesn't return
					jsr		_panic


_vbl_isr:			movem.l	d0-2/a0-2,-(sp)
					jsr		_do_vbl_isr
					movem.l	(sp)+,d0-2/a0-2
					jsr		pre_return
					rte
;
; subroutines that set and clear _resched_needed
; do_timer_A_isr	YES
; _yield			YES
; do_read			YES_BLOCK
; schedule			NO
; 
; Called after each syscall etc to check if a resched is needed.
; 
; pre_return checks if a reschedule is needed then calls 
; UINT16 * const curr_proc      = 0x000200;
; UINT16 * const resched_needed = 0x000202;			; 0=no, 1=yes, 2=yes with eventual trap restart (blocking) 
; struct process * const proc   = 0x000204;			; array of MAX_NUM_PROC (4) process structures 
pre_return:			cmpi.w	#$2000,4(sp)			; check if S bit was set (xxSx xxxx xxxx xxxx)
					bhs.s	out2					; ... if invoked from kernel, just return (avoid nested ISRs)
					move.l	a0,-(sp)				; save the a0 register
					ori.w	#$0700,sr				; mask IRQs for remainder of return to user
					movea.l	_resched_needed,a0		; load the address of _resched_needed into a0
					tst.w	(a0)					; check if reschedule needed
					beq.s	out1					; ... if not, just return to original process
					cmpi.w	#2,(a0)					; check if resched_needed with trap restart (blocking) 
					bne.s	after_pc_adjust			; if not blocking go to after_pc_adjust
					subi.l	#2,10(sp)				; if trap blocked process, it will need to be re-started
after_pc_adjust:	movea.l	_curr_proc,a0			; switch to next process
					move.l	d0,-(sp)				; save d0
					move.w	(a0),d0					; load the process id into d0 
					mulu.w	#PROCESS_ENTRY_SIZE,d0	; calculate the offset 
					movea.l	_proc,a0				; load the base address of the process table into a0
					lea		(a0,d0.w),a0			; calculate the address of the process and store it in a0
					move.l	(sp)+,d0				; restore d0
					jsr		_store_cpu_context		; store the cpu context of the process
					jsr		_schedule				; doesn't return
out1:				movea.l	(sp)+,a0				; restore a0
out2:				rts								; return

_timer_A_isr:		movem.l	d0-2/a0-2,-(sp)
					jsr		_do_timer_A_isr
					movem.l	(sp)+,d0-2/a0-2
					jsr		pre_return
					rte
					
_ikbd_isr:			movem.l	d0-2/a0-2,-(sp)
					jsr		_do_ikbd_isr
					movem.l	(sp)+,d0-2/a0-2
					jsr		pre_return
					rte

_create_process:	link	a6,#0
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
					jsr		_do_create_process
					addq.l	#4,sp
					movem.l	(sp)+,d0-2/a0-2
					jsr		pre_return
					rte
					
_exit:				trap	#1						; no parameter
					
_sys_exit:			jsr		_do_exit				; doesn't return
					jsr		_panic

_write:				link	a6,#0
					move.w	12(a6),-(sp)
					move.l	8(a6),-(sp)
					trap	#3
					unlk	a6
					rts

_sys_write:			move.l	usp,a0
					move.w	4(a0),-(sp)
					move.l	(a0),-(sp)
					jsr		_do_write
					addq.l	#6,sp
					jsr		pre_return
					rte
					
_read:				link	a6,#0
					move.w	12(a6),-(sp)
					move.l	8(a6),-(sp)
					trap	#4						; returns with output in d0.w
					unlk	a6
					rts
					
_sys_read:			;movem.l	d0-2/a0-2,-(sp)		; [TO DO] simplify reg save/restore for exceptions & traps
					move.l	usp,a0
					move.w	4(a0),-(sp)
					move.l	(a0),-(sp)
					jsr		_do_read				; returns with output in d0.w
					addq.l	#6,sp
					;movem.l	(sp)+,d0-2/a0-2
					jsr		pre_return
					rte

_get_pid:			trap	#5						; returns with output in d0.w
					rts
					
_sys_get_pid:		jsr		_do_get_pid				; returns with output in d0.w
					jsr		pre_return
					rte

_yield:				trap	#7
					rts
					
_sys_yield:			jsr		_do_yield				; doesn't return
					jsr		pre_return
					jsr		_panic
					
_read_SR:			move.w	sr,d0
					rts

_write_SR:			move.w	4(sp),sr
					rts

_await_interrupt:	stop	#$2200
					rts

base	equ		64									; offset from SP, not A6

_clear_screen:		movem.l	d0-7/a0-6,-(sp)
					lea		zeros,a0
					movem.l	(a0)+,d1-7/a1-6
					movea.l	base(sp),a0
					adda.l	#32000,a0
					move.w	#614,d0
fill_loop:			movem.l	d1-7/a1-6,-(a0)
					dbra	d0,fill_loop
					movem.l	d1-5,-(a0)
					movem.l	(sp)+,d0-7/a0-6
					rts

zeros:				ds.l	13

_scroll:			movem.l	d3-7/a3-5,-(sp)
					movea.l	#VIDEO_BASE,a1
					movea.l	#VIDEO_BASE+640,a0
					move.w	#783,d0
					move.l	#40,d1
scroll_most:		movem.l	(a0)+,d2-d7/a2-5
					movem.l	d2-7/a2-5,(a1)
					adda.l	d1,a1
					dbra	d0,scroll_most
					movea.l	#VIDEO_BASE+32000,a1
					lea		zeros,a0
					movem.l	(a0),d2-7/a2-5
					moveq.w	#15,d0
clear_rest:			movem.l	d2-7/a2-5,-(a1)
					dbra	d0,clear_rest
					movem.l	(sp)+,d3-7/a3-5
					rts

_load_cpu_context:	movea.l	4(sp),a0	; Load the address of CPU_context struct
	movea.l	#OS_RAM_TOP,sp	; Set kernel stack pointer
	move.l	(a0)+,-(sp)	; Move 4 bytes (PC)
	move.w	(a0)+,-(sp)	; Move 2 bytes (SR)
	movea.l	(a0)+,a1	; Move 4 bytes (USP)
	move.l	a1,usp		; Set User Stack Pointer CPU_context->usp
	movem.l	(a0)+,d0-7/a1-6	; Move 56 bytes (d0-d7, a1-a6)
	movea.l	(a0),a0		; Move 4 bytes (a0) from a0 to a0
	rte			; Return From Exception 
				; (pops PC and SR from kernel stack)

; PART OF SWITCH FROM PROCESS - CALLED FROM ISR WITH STACK (TOP-DOWN) AS:
;
;	saved PC
;	saved SR
;	return address (back to ISR)
;	saved A0
;	return address (back to pre_return)
;
;... AND MUST BE CALLED WITH CPU D0-7/A1-6 EXACTLY AS-WAS AT START OF ISR
;... AND A0 pointing to Context struct start addr
;... AND can't be called by address/bus error ISR (extra data on stack)

_store_cpu_context:	adda.l	#CPU_CONTEXT_SIZE,a0
					move.l	4(sp),-(a0)
					movem.l	d0-7/a1-6,-(a0)
					move.l	usp,a1
					move.l	a1,-(a0)
					move.w	12(sp),-(a0)
					move.l	14(sp),-(a0)
					rts
