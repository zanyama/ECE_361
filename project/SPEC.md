# SPEC: fuel pump

## States
IDLE (initial), AUTHORIZED, PUMPING, PAUSED, COMPLETE

## RGB Colors
| State      | Color  | Red |Green| Blue|
|------------|--------|-----|-----|-----|
| IDLE       | blue   |  0  |  0  | 255 |
| AUTHORIZED | yellow | 255 | 255 |  0  |
| PUMPING    | green  |  0  | 255 |  0  |
| PAUSED     | red    | 255 |  0  |  0  |
| COMPLETE   | white  | 255 | 255 | 255 |

## Events
From the switches:
- CARD: card accepted (press, bit 0 going 0 to 1)
- TRIGGER_ON: handle squeezed (bit 1 on)
- TRIGGER_OFF: handle released (bit 1 off)
- HANG_UP: nozzle back in its holder (press, bit 2 going 0 to 1)

From time and the sale:
- AUTH_TIMEOUT: 60 ticks in AUTHORIZED or PAUSED without TRIGGER
- LIMIT_REACHED: 40 liters pumped
- DONE_TIMEOUT: 5 ticks in COMPLETE

From the temperature:
- TEMP_HIGH: above 45.0 C
- TEMP_OK: at or below 42.0 C

## Decisions
- CARD during PUMPING: ignore- finish the current transaction and return to IDLE
  before a new card can be swiped.

- TRIGGER squeezed while PAUSED for vapor: ignored. Each squeeze is a
  session, and every session needs to be under the given temperature threshold. A session that began above the threshold never actually pumps. Pumping
  resumes only on a new squeeze after the temperature is back at or
  below 42.0 C; TEMP_OK alone stays in PAUSED.

- HANG_UP while AUTHORIZED: cancels the sale (back to IDLE, lamp off),
  the same as AUTH_TIMEOUT. The timeout only guesses the customer left;
  hanging up says so, and it closes the window where someone else could
  pump on their card.

- TEMP_HIGH while AUTHORIZED: goes to PAUSED, the same place PUMPING goes
  when it gets too hot. The motor is not on yet, so the action is NONE;
  only the color changes. The sale stays open: once it is back at or
  below 42.0 C, a new squeeze starts pumping.

- HANG_UP while PAUSED: ends the sale (COMPLETE, lamp off; the motor is
  already off), even if 0 liters were pumped. Hanging up says the
  customer is done, the same as it does while PUMPING, and COMPLETE
  shows whatever was pumped, 0 included.

- AUTH_TIMEOUT while PAUSED: regardless of how much has been pumped, complete
  the sale (COMPLETE, lamp off). If that much time has passed, we can assume
  the customer is done. The 60 ticks count only while it is not too hot, so a
  vapor pause never ends a sale on its own.

## Records and the store
`record_t` (in `store.h`): the `reading_t`, the `fsm_state_t` at that tick,
and the `uint32_t` status word. Key: humidity rounded down to a whole
percent. For now the store is a sorted array; equal keys go after the
ones already there.


