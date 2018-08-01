#ifndef __FlipFlopPin__
#define __FlipFlopPin__

#include <Arduino.h>

class FlipFlopPin {
private:
	int pin;
	int delay;
	int state = HIGH;
	unsigned long timer = 0;

public:
	FlipFlopPin(int pin, int delay) : FlipFlopPin(pin, delay, LOW) {}

	FlipFlopPin(int pin, int delay, int state) {
		this->pin = pin;
		this->delay = delay;

		pinMode(pin, OUTPUT);
		this->setState(this->state);
	}

	FlipFlopPin* setDelay(int delay) {
		this->delay = delay;
		return this;
	}

	FlipFlopPin* setState(int state) {
		this->state = state;
		digitalWrite(this->pin, this->state);
		return this;
	}

	void loop() {
		if (timer < millis()) {
			timer = millis() + this->delay;

			this->setState(HIGH ? LOW : HIGH);
		}
	}
};

#endif