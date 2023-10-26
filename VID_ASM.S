		xdef	_scroll
		xdef	_clear_screen

VIDEO_BASE	equ	$3F8000
base		equ	64	; offset from SP, not A6

_scroll:
_clear_screen:
	movem.l	d0-7/a0-6,-(sp)
	lea	zeros,a0
	movem.l	(a0)+,d1-7/a1-6
	movea.l	base(sp),a0
	adda.l	#32000,a0
	move.w	#614,d0
fill_loop:
	movem.l	d1-7/a1-6,-(a0)
	dbra	d0,fill_loop
	movem.l	d1-5,-(a0)
	movem.l	(sp)+,d0-7/a0-6
	rts

_scroll:
	movem.l	d3-7/a3-5,-(sp)
	movea.l	#VIDEO_BASE,a1
	movea.l	#VIDEO_BASE+640,a0
	move.w	#783,d0
	move.l	#40,d1
scroll_most:
	movem.l	(a0)+,d2-d7/a2-5
	movem.l	d2-7/a2-5,(a1)
	adda.l	d1,a1
	dbra	d0,scroll_most
	movea.l	#VIDEO_BASE+32000,a1
	lea	zeros,a0
	movem.l	(a0),d2-7/a2-5
	moveq.w	#15,d0
clear_rest:
	movem.l	d2-7/a2-5,-(a1)
	dbra	d0,clear_rest
	movem.l	(sp)+,d3-7/a3-5
	rts

zeros:	ds.l	13

