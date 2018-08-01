#include <FlipFlopPin.h>

#define STATUS_LED LED_BUILTIN

FlipFlopPin* statusLED;

void setup() {
  // pinMode(STATUS_LED, OUTPUT);
  statusLED = new FlipFlopPin(STATUS_LED, 1000);
}

// old style
void loopWithDelay() {
  digitalWrite(STATUS_LED, HIGH);
  delay(500);
  digitalWrite(STATUS_LED, LOW);
  delay(500);
}

void loop() {
  statusLED->loop();
}


