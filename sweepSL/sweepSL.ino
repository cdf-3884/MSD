/*
  Name:         sweepSL.ino
  Created:      02-04-2026
  Author:       Charles French
  Description:  sweep the range in which an LAC can operate
*/

int pinPWM = 9; // PWM pin

/* setup() runs once */
void setup() {
  pinMode(pinPWM, OUTPUT);
}

/* loop() runs until program terminates */
void loop() {
  for (int i = 0; i <= 255; i++) {
    analogWrite(pinPWM, i); // increase stroke length (LAC position)
    delay(100);
  }

  for (int i = 255; i >= 0; i--) {
    analogWrite(pinPWM, i); // decrease stroke length (LAC position)
    delay(100);
  }

  exit(EXIT_SUCCESS);
}
