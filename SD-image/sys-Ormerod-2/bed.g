; bed.g
; called to perform automatic bed compensation
M561
G1 Z5 F360
G1 X40 Y10 F12000
G30 P0 Z-10000
G1 Z5 F360
G1 X40 Y185 F12000
G30 P1 Z-10000
G1 Z5 F360
G1 X204 Y185 F12000
G30 P2 Z-10000
G1 Z5 F360
G1 X204 Y10 F12000
G30 P3 Z-10000 S
;G1 Z5 F360
;G1 X122 Y97.5 F12000
;G30 P4 Z-10000 S
G1 Z5 F360
