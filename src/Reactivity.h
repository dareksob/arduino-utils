#include <Sensor.h>

template<class T>
class Reactivity {
  T value;
  Enableable sensor;

  public:
  Reactivity(Enableable* sensor) {
    this->sensor = sensor;
    this->value = sensor->isEnable();
  }

  T getValue() {
    return this->value;
  }

  bool updated() {
    T current = this->sensor->isEnable();
    bool updated = false;

    if (current != this->value) {
      updated = true;
      this->value = current;
    }

    return updated;
  }
}
