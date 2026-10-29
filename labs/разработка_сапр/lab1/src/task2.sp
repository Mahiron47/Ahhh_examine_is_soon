*RC-net

V0 in 0 PULSE(0 5 2n 1n 1n 10n 20n)
R0 in out 1K
C0 out 0 1p

.TRAN 1n 100n
.PROBE V(in) V(out)
.END
