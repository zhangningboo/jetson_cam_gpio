#ifndef JETSON_CAM_GPIO_HALL_SENSOR_H
#define JETSON_CAM_GPIO_HALL_SENSOR_H

#include <thread>
#include <mutex>

class HallSensor {
private:
    int gpio_pin;
    int pin_state;

    std::shared_ptr<std::mutex> lock = std::make_shared<std::mutex>();

public:
    HallSensor(int gpio_pin);

    ~HallSensor();

    std::thread hall_thread;
    
    void open_hall();
    
    void get_hall_state(int &state);
};
#endif //JETSON_CAM_GPIO_HALL_SENSOR_H
