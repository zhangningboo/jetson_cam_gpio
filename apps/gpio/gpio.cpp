#include <iostream>

#include "JetsonGPIO.h"

#include "fmt/format.h"

int main() {

    GPIO::setmode(GPIO::BOARD);
    GPIO::NumberingModes mode = GPIO::getmode();

    int pin_num = 16;
    GPIO::setup(pin_num, GPIO::IN);

    int pre_value = GPIO::input(pin_num);

    int cnt = 0;

    do {
        int current_value = GPIO::input(pin_num);
        if (pre_value != current_value) {
            fmt::print("state change to: {}\n", current_value);
            pre_value = current_value;
            // cnt += 1;
        }
    } while (cnt < 10);

    GPIO::cleanup();
    return 0;
}