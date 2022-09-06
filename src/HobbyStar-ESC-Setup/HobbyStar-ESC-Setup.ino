/*
  Sets up HobbyStar ESC with Arduino, in accordance with  the instructions from the manual
  https://hobbystarlabs.com/hobbystar-120a-brushless-sensored-esc-2-6s.html#.YwcNC3bMIuU

  Button 1 sets maximum.
  Button 2 sets minimum (reverse).
  Pressing both will set the neutral.

  Pinout is set to Arduino Due
*/

#include <Servo.h>

// constants won't change. They're used here to set pin numbers:
const int buttonOnePin = 2;     // the number of the pushbutton pin
const int buttonTwoPin = 3;     // the number of the pushbutton pin

Servo ESC;

// variables will change:
int buttonOneState = 0;         // variable for reading the pushbutton 1 status
int buttonTwoState = 0;         // variable for reading the pushbutton 2 status

void setup() {
  // initialize the ESC
  ESC.attach(9, 1000, 2000);
  // initialize the pushbutton pin as an input:
  pinMode(buttonOnePin, INPUT);
  pinMode(buttonTwoPin, INPUT);
}

void loop() {
  // read the state of the pushbutton value:
  buttonOneState = digitalRead(buttonOnePin);
  buttonTwoState = digitalRead(buttonTwoPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if ((buttonOneState == HIGH) && (buttonTwoState == LOW)) {
    ESC.writeMicroseconds(2000);
  }

  else if ((buttonOneState == LOW) && (buttonTwoState == HIGH)) {
    ESC.writeMicroseconds(1000);
  }

  else if ((buttonOneState == HIGH) && (buttonTwoState == HIGH)) {
    ESC.writeMicroseconds(1500);
  }
}
