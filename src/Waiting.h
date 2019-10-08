#ifndef __Waiting__
#define __Waiting__

#include <Arduino.h>

class Waiting {
private:
	unsigned long next;
	unsigned long time;
public:
	Waiting(int time) {
		this->time = time;
		this->update();
	}

	Waiting* reset() {
		this->update();
		return this;
	}

	unsigned long getNextTime() {
		return this->next;
	}

	unsigned long getTime() {
		return this->time;
	}

	bool run() {
		if (this->next < millis()) {
			this->update();
			return true;
		}

		return false;
	}

private: 
	void update() {
		this->next = millis() + time;

		/*
		Serial.print("Waiting ");
		Serial.println(this->next);
		*/
	}
};

#endif