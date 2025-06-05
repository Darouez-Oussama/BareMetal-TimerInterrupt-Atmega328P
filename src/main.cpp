#include <Arduino.h>
#include "timerInterrupt.h"
static int i = 0;
void setup() {
  Serial.begin(9600);
  timerInterruptSetup(2.0); 
}
ISR(TIMER1_COMPA_vect){
   // Write the sinewave points, followed by the terminator "Carriage Return" and "Linefeed".
  Serial.print(sin(i*50/360.0));
  Serial.write(13);
  Serial.write(10);
  i += 1;
}

void loop() {


}
