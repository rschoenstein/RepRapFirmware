; bed.g
; called to perform automatic bed compensation
M561
G1 Z5 F200
G1 X40 Y20 F12000
G30 P0 Z-10000
G1 Z5 F200
G1 X40 Y200 F12000
G30 P1 Z-10000
G1 Z5 F200
G1 X210 Y200 F12000
G30 P2 Z-10000
G1 Z5 F200
G1 X210 Y20 F12000
G30 P3 Z-10000 S
;G1 Z5 F200
;G1 X125 Y110 F12000
;G30 P4 Z-10000 S
G1 Z5 F200