# SPEC: fuel pump

## States
IDLE (initial), AUTHORIZED, PUMPING, PAUSED, COMPLETE

## Events
From the switches:
- CARD: card accepted (press, bit 0 going 0 to 1)
- TRIGGER_ON: handle squeezed (bit 1 on)
- TRIGGER_OFF: handle released (bit 1 off)
- HANG_UP: nozzle back in its holder (press, bit 2 going 0 to 1)

From time and the sale:
- AUTH_TIMEOUT: 60 ticks in AUTHORIZED without TRIGGER
- LIMIT_REACHED: 40 liters pumped
- DONE_TIMEOUT: 5 ticks in COMPLETE

From the temperature:
- TEMP_HIGH: above 45.0 C
- TEMP_OK: at or below 42.0 C

## To decide (week 5)
- CARD during PUMPING
- HANG_UP while AUTHORIZED
- TRIGGER squeezed while PAUSED for vapor
