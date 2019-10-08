#ifndef __SENDOR__
#define __SENDOR__

#include <Arduino.h>
#include <DHT.h>

/**
 * Pinable interface for pin based sensor
 */
class Pinable
{
protected:
	unsigned int pin;
	unsigned int mode;

	Pinable(unsigned int pin, unsigned int mode)
	{
		this->pin = pin;
		this->mode = mode;
		pinMode(pin, mode);
	};

	Pinable(unsigned int pin)
	{
		this->pin = pin;
	};
};

/**
 * Enableable Sensor behaivor
 */
class Enableable
{
protected:
	bool enable = false;

public:
	bool isEnable()
	{
		return this->enable;
	};

	Enableable *on(void)
	{
		return this->setState(true);
	};

	Enableable *off(void)
	{
		return this->setState(false);
	};

	Enableable *setState(bool on)
	{
		this->enable = on;
		return this;
	}
};

/**
 * Simple Trigger sensor
 * for check if trigger is pressed
 */
class TriggerSensor : protected Pinable
{
public:
	TriggerSensor(unsigned int pin) : Pinable(pin, INPUT){};

	bool isEnable()
	{
		return digitalRead(this->pin) > 0;
	};
};

/**
 * SwitchSensor
 */
class SwitchSensor : protected Pinable, public Enableable
{
public:
	SwitchSensor(unsigned int pin) : Pinable(pin, OUTPUT){};

// @todo not overwrite to call internal state function
	SwitchSensor *on(void)
	{
		return this->setState(true);
	};

	SwitchSensor *off(void)
	{
		return this->setState(false);
	};

	SwitchSensor *setState(bool on)
	{
		if (on)
		{
			digitalWrite(this->pin, HIGH);
		}
		else
		{
			digitalWrite(this->pin, LOW);
		}

		Enableable::setState(on);
		return this;
	};
};

/**
 * PingSwitchSensor similar to switch but will be disable automaticly
 */
class PingSwitchSensor : protected Pinable, public Enableable
{
	unsigned int wait = 5;

public:
	PingSwitchSensor(unsigned int pin) : Pinable(pin, OUTPUT){};
	PingSwitchSensor(unsigned int pin, unsigned int wait) : Pinable(pin, OUTPUT)
	{
		this->wait = wait;
	};

	PingSwitchSensor *setState(bool on)
	{
		digitalWrite(this->pin, LOW);
		Enableable::setState(false);

		// single time call
		if (on)
		{
			Enableable::setState(true);
			digitalWrite(this->pin, HIGH);
			delay(this->wait);
			this->setState(false);
		}

		return this;
	};
};

class AlarmSensor : protected Pinable, public Enableable
{
public:
	AlarmSensor(unsigned int pin) : Pinable(pin){};

	AlarmSensor *setState(bool on)
	{
		if (on)
		{
			tone(this->pin, 100, 1000 / 5);
		}
		else
		{
			noTone(this->pin);
		}

		this->enable = on;
		return this;
	}
};

class UltrasonicSensor
{
	unsigned int pinTrigger;
	unsigned int pinEcho;
	long distance = 0;
	long duration = 0;

public:
	UltrasonicSensor(unsigned int triggerPin, unsigned int echoPin)
	{
		this->pinTrigger = triggerPin;
		this->pinEcho = echoPin;

		pinMode(triggerPin, OUTPUT);
		pinMode(echoPin, INPUT);
	};

	UltrasonicSensor *update()
	{
		digitalWrite(this->pinTrigger, LOW);
		delay(5);
		digitalWrite(this->pinTrigger, HIGH);
		delay(10);
		digitalWrite(this->pinTrigger, LOW);
		this->duration = pulseIn(this->pinEcho, HIGH);
		this->distance = (this->duration / 2) * 0.03432;
		return this;
	};

	bool isValid()
	{
		long distance = this->distance;
		//Wenn die gemessene Entfernung über 500cm oder unter 0cm liegt,…
		//dann soll der serial monitor ausgeben „Kein Messwert“, weil Messwerte in diesen Bereichen falsch oder ungenau sind.
		return distance <= 500 || distance >= 0;
	};

	long getDistance()
	{
		return this->distance;
	};
};

class TemperatureHumiditySensor
{
	float humidity = 0;
	float temperature = 0;
	DHT *adapter;

public:
	TemperatureHumiditySensor(unsigned int pin, uint8_t type)
	{
		this->adapter = new DHT(pin, type);
		this->adapter->begin();
	}

	bool isValid()
	{
		return (isnan(this->temperature) && isnan(this->humidity)) == false;
	}

	TemperatureHumiditySensor *update()
	{
		this->humidity = this->adapter->readHumidity();
		this->temperature = this->adapter->readTemperature();
	}

	float getHumidity()
	{
		return this->humidity;
	};

	float getTemperature()
	{
		return this->temperature;
	};
};

#endif