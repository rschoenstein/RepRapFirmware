; Configuration file for RepRap Huxley Duo
; RepRapPro Ltd
;
; Copy this file to config.g if you have an Huxley Duo
; If you are updating a config.g that you already have you
; may wish to go through it and this file checking what you
; want to keep from your old file.
; 
M111 S0                             ; Debug off
M550 PRepRapPro Huxley Duo          ; Machine name (can be anything you like)
M551 Preprap                        ; Machine password (currently not used)
M540 P0xBE:0xEF:0xDE:0xAD:0xFE:0x14 ; MAC Address
M552 P192.168.1.14                  ; IP address
M553 P255.255.255.0                 ; Netmask
M554 P192.168.1.1                   ; Gateway
M555 P2                             ; Set output to look like Marlin
G21                                 ; Work in millimetres
G90                                 ; Send absolute corrdinates...
M83                                 ; ...but relative extruder moves
M574 X0 Y1 Z0 S1                    ; set endstop configuration (Y endstop only, at low end, active high)
M906 X600 Y600 Z600 E600            ; Set motor currents (mA)
;M305 P0 R4700 H0 L0                 ; Set the heated bed thermistor series resistor to 4K7
;M305 P1 R4700 H0 L0                 ; Set the hot end thermistor series resistor to 4K7
;M305 P2 R4700 H0 L0                 ; Set the second hot end thermistor series resistor to 4K7
M569 P0 S1                          ; Make X axis go forwards
M569 P3 S0                          ; Make Z axis go backwards
M92 E660:660                        ; Set extruder steps per mm
M558 P2                             ; Use a modulated Z probe
G31 Z0.6 P550                       ; Set the probe height and threshold (deliberately too high to avoid bed crashes on initial setup)
M556 S78 X0 Y0 Z0                   ; Put your axis compensation here
M201 X3000 Y3000 Z150 E750          ; Accelerations (mm/s^2)
M203 X15000 Y15000 Z100 E3600       ; Maximum speeds (mm/min)
M566 X1400 Y1400 Z30 E20            ; Maximum instantaneous speed changes mm/minute
M305 P0 T100000                     ; set bed thermistor resistance at 25C
M208 X-13 Y-4 S1                    ; Set axis minimum (adjust to make X=0 the edge of the bed)
M208 X130 Y138                      ; Set axis maxima (adjust to suit your machine)
M563 P0 D0 H1                       ; Define tool 0
G10 P0 S-273 R-273                  ; Set tool 0 operating and standby temperatures
;M563 P1 D1 H2                       ; Define tool 1 Uncomment if you have a dual colour upgrade
;G10 P1 X19 S-273 R-273              ; Set tool 1 operating and standby temperatures  Uncomment if you have a dual colour upgrade
