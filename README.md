## 🌐 Pill Dispenser Project
This system was made as a final project for `Embedded Systems Programming` course by Sami Barbaglia, Milja Hotanen, and Lucas Lamponen. 
The system integrates **Raspberry Pi Pico WH**, an **optical fork sensor**, and a **piezoelectric sensor**, as well as a dispenser wheel (with eight slots) and a base.

The image below illustrates the hardware used, though it also shows an EEPROM and a LoRaWAN that were **not utilized in this project**.

<img width="902" height="946" alt="Image" src="https://github.com/user-attachments/assets/5e46a228-2e44-4f5e-a846-cc04addd0b93" />

### 🔗 Description

When the program is turned on, the Pico WH will blink a LED until the user presses the `sw_0 / calibration` button. 
To find its starting place, the dispenser wheel will turn until light from an opening on the side of the wheel is detected. 

The wheel will be then turned around another full round until light is detected again to secure the calibration. 
The system will then wait for the user to press the `rotary / dispensing` button.

When done so, the wheel will move one slot. The program waits 40ms for the piezoelectric sensor on the bottom of the drop 
to trigger (pill falls on it) and will print either `DAY X: Pill dispenced` or `DAY X: NO PILL DETECTED`. 

If a pill is detected, the program will wait thirty seconds (for testing purposes, otherwise 24 hours) to dispense the next one.
If no pill is detected, the same timer for thirty seconds will begin while the LED blinks five times as warning for no pill detected.

This process repeats seven times (a full turn, and a week of medication). 
When all the slots have been dispensed, the program asks to be calibrated again and the LED blinks until this is done.

### 🔧 Features & Usability

| Button   | Function                                   |
|----------|--------------------------------------------|
| `sw_0`   | Calibrate starting spot for wheel          |
| `rotary` | Begins dispensing pills one slot at a time |
