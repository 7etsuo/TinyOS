; schd_asm.s
			xdef	_load_cpu_context
			xdef	_store_cpu_context
			xdef	_pre_return
; c refs
			xref	_kernel_stack_top	; initial SSP 
			xref	_proc,_curr_proc	; struct process 102 bytes
			xref	_resched_needed		; kernel data variable 
			xref	_schedule		; function

CPU_CONTEXT_SIZE	equ		70
PROCESS_ENTRY_SIZE	equ		102

; subroutines that set and clear _resched_needed
; do_timer_A_isr	YES
; _yield		YES
; do_read		YES_BLOCK
; schedule		NO
; 
; Called after each syscall etc to check if a resched is needed.
; 
; pre_return checks if a reschedule is needed then calls 
; UINT16 * const curr_proc      = 0x000200;
; UINT16 * const resched_needed = 0x000202;	; 0=no, 1=yes, 2=yes with eventual trap restart (blocking) 
; struct process * const proc   = 0x000204;	; array of MAX_NUM_PROC (4) process structures 
_pre_return:
	cmpi.w	#$2000,4(sp)		; check if S bit was set (xxSx xxxx xxxx xxxx)
	bhs.s	out2			; ... if invoked from kernel, just return (avoid nested ISRs)
	move.l	a0,-(sp)		; save the a0 register
	ori.w	#$0700,sr		; mask IRQs for remainder of return to user
	movea.l	_resched_needed,a0	; load the address of _resched_needed into a0
	tst.w	(a0)			; check if reschedule needed
	beq.s	out1			; ... if not, just return to original process
	cmpi.w	#2,(a0)			; check if resched_needed with trap restart (blocking) 
	bne.s	after_pc_adjust		; if not blocking go to after_pc_adjust
	subi.l	#2,10(sp)		; if trap blocked process, it will need to be re-started
after_pc_adjust:
	movea.l	_curr_proc,a0		; switch to next process
	move.l	d0,-(sp)		; save d0
	move.w	(a0),d0			; load the process id into d0 
	mulu.w	#PROCESS_ENTRY_SIZE,d0	; calculate the offset 
	movea.l	_proc,a0		; load the base address of the process table into a0
	lea	(a0,d0.w),a0		; calculate the address of the process and store it in a0
	move.l	(sp)+,d0		; restore d0
	jsr	_store_cpu_context	; store the cpu context of the process
	jsr	_schedule		; doesn't return
out1:
	movea.l	(sp)+,a0		; restore a0
out2:
	rts				; return

; dispatcher
_load_cpu_context:
	movea.l	4(sp),a0	; Load the address of CPU_context struct
	movea.l	_kernel_stack_top,sp	; set up the kernel stack pointer.initial SSP
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
_store_cpu_context:
	adda.l	#CPU_CONTEXT_SIZE,a0
	move.l	4(sp),-(a0)
	movem.l	d0-7/a1-6,-(a0)
	move.l	usp,a1
	move.l	a1,-(a0)
	move.w	12(sp),-(a0)
	move.l	14(sp),-(a0)
	rts

