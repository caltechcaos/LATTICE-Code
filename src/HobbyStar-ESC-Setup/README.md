The following code is meant to set up the HobbyStar 120 A ESC.

1) Wire the following circuit (will be linked when I put images in folder), **but** keep the Arduino signal wire *disconnected*.
![alt text](https://github.com/KPulungan/LATTICE-Code/blob/patch-1/src/HobbyStar-ESC-Setup/ESC%20calibration_schem.png)
2) With the ESC switched off, attach the ESC to 18 V from the benchtop power supply, and power on the Arduino.
3) Press the button attached to pin 2 (activates fastest forwards PWM signal).
4) Hold the small "set" button down and turn on ESC while still holding the "set" button. After about 4 seconds the LED will turn SOLID BLUE.
5) Plug in the Arduino signal wire, and the LED will FLASH RED and then turn SOLID RED.
6) Then press the button attached to pin 3 (activates fastest backwards PWM signal). The LED will FLASH BLUE and then turn SOLID BLUE.
7) Then press **and hold** the two buttons until both the blue and red lights flash, turn solid, then turn off.
8) Turn off ESC, then back on.
