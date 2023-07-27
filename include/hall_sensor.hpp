#ifndef JETSON_CAM_GPIO_HALL_SENSOR_H
#define JETSON_CAM_GPIO_HALL_SENSOR_H

#include <thread>
#include <mutex>

class HallSensor {
private:
    int gpio_pin;
    int pin_state;

    std::thread hall_thread;

    std::shared_ptr<std::mutex> lock = std::make_shared<std::mutex>();

public:
    HallSensor(int gpio_pin);

    ~HallSensor();

    void open_hall();
    
    void get_hall_state(int &state);
};
#endif //JETSON_CAM_GPIO_HALL_SENSOR_H
