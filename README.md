# Pill dispenser
<p>Project for "Embedded Systems Programming" course</p>
<p><b>Programming language:</b> C</p>
<p><b>Hardware:</b> Rasperry Pi Pico and SDK, Optical fork for calculating a full turn by detecting light, Piezo sensor for detecting a dispensed pill by weight (aka press against the sensor), Raspberry Pi Debugger</p>

<p>Short overview:</p>
When the program is turned on, it will blink a LED until the user presses the calibration button. The dispencing wheel will turn until light is detected and then turn a full round to secure the calibration.
It will then wait for the user to press the dispencing button. When done so, the first pill is detected and the program prints either "DAY 1: Pill dispenced" or "DAY 1: NO PILL DETECTED". 
If a pill is detected, the program will wait thirty seconds (for testing purposes, othervice 24 hours) to dispence the next one.
If no pill is detected, the timer for thirty seconds will begin while the LED blinks five times as warning. This process repeats seven times (a full turn, and a week of medication). 
When all the slots have been dispensed, the program asks to be calibrated again and the LED blinks until calibrated again.
