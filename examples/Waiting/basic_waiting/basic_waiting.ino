#include "Waiting.h"

#define SERIAL_PORT 9600

Waiting* wait = new Waiting(1000);


void setup() {
  Serial.begin(SERIAL_PORT);
}

void loop() {
  if (wait->run()) {
    Serial.println("Recall after time");
  }
}
