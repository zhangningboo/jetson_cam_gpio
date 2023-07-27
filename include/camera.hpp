#ifndef JETSON_CAM_GPIO_CAMERA_H
#define JETSON_CAM_GPIO_CAMERA_H

#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <ctime>
#include "opencv2/opencv.hpp"
#include "hall_sensor.hpp"

struct FrameResult {
    cv::Mat frame;
    double timestamp;
    time_t system_timestamp;
};

class Camera {
public:

    std::thread camera_thread;

    void open_camera(std::string &camera);

    void get_frame(FrameResult &frame_result);

    void push_frame(cv::Mat &frame, double timestamp, time_t system_timestamp);

private:

    struct RingQueue {
        const static int queue_len = 10;
        cv::Mat frames[queue_len] = {};
        double timestamps[queue_len] = {};
        time_t system_timestamp[queue_len] = {};
        int tail = 0;
    };

    std::shared_ptr<std::mutex> lock = std::make_shared<std::mutex>();

    RingQueue ring_queue;

};

// void process_frame(Camera &cam, HallSensor &hall_sensor, bool save_frame, bool show_frame, std::string& dst_dir);

void process_frame(Camera &lcam, Camera &rcam, HallSensor &lhall, HallSensor &rhall, std::string &dst_dir);

#endif //JETSON_CAM_GPIO_CAMERA_H
