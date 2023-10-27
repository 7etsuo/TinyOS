; proc_asm.s
			xdef	_read_SR,_write_SR
			xdef	_await_interrupt
_await_interrupt:
	stop	#$2200
	rts

_read_SR:
	move.w	sr,d0
	rts

_write_SR:
	move.w	4(sp),sr
	rts

