#ifndef __Waiting__
#define __Waiting__

#include <Arduino.h>

class Waiting {
private:
	unsigned int next;
	unsigned int time;
public:
	Waiting(int time) {
		this->time = time;
		this->update();
	}

	Waiting* reset() {
		this->update();
		return this;
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
	}
};

#endif