#include <iostream>

#include "JetsonGPIO.h"

#include "fmt/format.h"


void print_state(int &pre_state, int &pin_num, std::string flag) {
    int current_state = GPIO::input(pin_num);
    if (pre_state != current_state) {
        fmt::print("{} state change to: {}\n", flag, current_state);
        pre_state = current_state;
    }
}


int main() {

    GPIO::setmode(GPIO::BOARD);
    GPIO::NumberingModes mode = GPIO::getmode();

    int lpin_num = 16;
    int rpin_num = 18;
    GPIO::setup(lpin_num, GPIO::IN);
    GPIO::setup(rpin_num, GPIO::IN);

    int lpre_state = GPIO::input(lpin_num);
    int rpre_state = GPIO::input(lpin_num);

    int cnt = 0;

    while (true) {
        print_state(lpre_state, lpin_num, "left");
        print_state(rpre_state, rpin_num, "right");
    }

    GPIO::cleanup();
    return 0;
}