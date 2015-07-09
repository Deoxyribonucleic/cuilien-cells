; This is a simple "seed" for the Cuilien
; cell simulation. To make it at least
; somewhat interesting right off the bat,
; we start with a simple loop that causes
; the cell to wander in a circle, eat,
; and split once its mass is above a
; certain threshold.
; I've also thrown some NOPs in there
; to increase the chances of a mutation hitting
; our code.

nop
nop
nop
nop
nop
nop
nop

LOOP:
nop
nop
nop
nop

; interrupt 1 is for moving
; direction is in B (B % 4)
;div b, 4
;mov b, a

; move in random direction (INT 0 = rand)
; int 0
nop
nop
nop
nop
nop
int 1
nop
nop
nop
nop
nop
; next direction
;inc b
nop
nop
nop
nop
nop
int 2 ; eat what we're standing on
nop
nop
nop
nop
nop
int 3 ; put mass in A
nop
nop
nop
nop
nop
cmp a, 10
nop
nop
nop
nop
nop
; if a < 10
jlt LOOP
nop
nop
nop
nop
nop
; else split
int 4
; jump for good measure
jmp LOOP
