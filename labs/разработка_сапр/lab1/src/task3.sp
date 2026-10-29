* Task3

.SUBCKT rc_net in out
R1 in out 1k
C1 out 0 10p
.ENDS rc_net

V0 1 0 PULSE(0 5 2u 50n 50n 5u 10u)
X1 1 2 rc_net
X2 2 3 rc_net
X3 3 4 rc_net

.TRAN 10n 20u
.END
