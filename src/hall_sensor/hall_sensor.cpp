#include "JetsonGPIO.h"
#include "hall_sensor.hpp"

HallSensor::HallSensor(int gpio_pin) {
    this->gpio_pin = gpio_pin;
}

HallSensor::~HallSensor() {
    GPIO::cleanup(this->gpio_pin);
}

void start_hall(int &pin_state, std::shared_ptr<std::mutex> lock, int gpio_pin) {
    GPIO::NumberingModes mode = GPIO::getmode();
    if (mode != GPIO::BOARD) {
         GPIO::setmode(GPIO::BOARD);
    }
    GPIO::setup(gpio_pin, GPIO::IN);
    int pre_value = GPIO::input(gpio_pin);
    while (true) {
        int current_value = GPIO::input(gpio_pin);
        if (pre_value != current_value) {
            pre_value = current_value;
            lock->lock();
            pin_state = current_value;
            lock->unlock();
        }
    }
}

void HallSensor::open_hall()  {
    this->hall_thread = std::thread(start_hall, std::ref(this->pin_state), this->lock, this->gpio_pin);
}

void HallSensor::get_hall_state(int &state)  {
    this->lock->lock();
    state = this->pin_state;
    this->lock->unlock();
}