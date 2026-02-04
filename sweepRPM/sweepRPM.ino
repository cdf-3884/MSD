/*
  Name:         sweepRPM.ino
  Created:      01-21-2026
  Author:       Charles French
  Description:  sweep the RPM range in which the DC motor can operate
*/
#include <VescUart.h>
#include <stdlib.h>

/* initialize a VescUart instance */
VescUart UART;

/* define maximum values according to engineering requirements and hardware limitations */
float maxAvgInputCurrent = 40.0;

/* define lower and upper bounds of RPM range due to engineering requirements, as well as
   the amount to step RPM up by per iteration, and the measured RPM from the VESC tool    */
float currentRPM = 300.0;  // start at 5 Hz
float maxRPM = 4200.0;  // end at 70 Hz
float rpmStep = 6.0;  // step by 0.1 Hz
float measuredRPM = 0.0;  // this value will come from the VESC tool

/* setup() runs once */
void setup() {
  /* set up debug port (to be read from a terminal) */
  Serial.begin(9600);

  while(!Serial) {;}
  /* defile Serial as debug port */
  UART.setDebugPort(&Serial);

  /* set up UART port */
  Serial1.begin(19200);

  while (!Serial1) {;}
  /* define Serial1 as UART port */
  UART.setSerialPort(&Serial1);
}

/* loop() runs until program terminates */
void loop() {
  delay(100);
  /* poll UART for VESC values */
  if (UART.getVescValues()) {

    /* make sure engineering requirements and hardware limitations are obeyed */
    if (UART.data.avgInputCurrent > maxAvgInputCurrent) {
      Serial.println("ERROR: Input current exceededs maximum input current of 40 Amps");
      exit(EXIT_FAILURE);
    }
    
    /* if this has been reached, engineering requirements and hardware limitations are obeyed */
    else {
      /* read in RPM measured by the VESC */
      measuredRPM = UART.data.rpm;

      /* make sure measured RPM is below or equal to maxRPM */
      if (measuredRPM > maxRPM) {
        Serial.println("SUCCESS: RPM sweep has successfully concluded");
        exit(EXIT_SUCCESS);
      }

      /* RPM needs to be increased if this statement is reached */
      else {
        /* print the target and measured RPM */
        Serial.print("INFO: target RPM: ");
        Serial.print(currentRPM);
        Serial.print(", measured RPM: ");
        Serial.println(measuredRPM);

        /* increase the target RPM by the RPM step value */
        currentRPM += rpmStep;

        /* send the new target RPM to the VESC */
        UART.setRPM(currentRPM);
      }
    }
  }

  /* UART.getVescValues() = 0 if failure to retrieve data occurs */
  else {
    Serial.println("ERROR: UART.getVescValues() failed to get data");
    exit(EXIT_FAILURE);
  }

}
