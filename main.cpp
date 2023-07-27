#include <iostream>
#include <queue>
#include <fstream>
#include <ctime>
#include <unistd.h>

#include "camera.hpp"
#include "hall_sensor.hpp"
#include "fmt/format.h"
#include "cmdline.h"

void parse_args(cmdline::parser &arg_parser) {
    arg_parser.add<std::string>("lcam", 'l', "left camera port, e.g  /dev/video0", true, "");
    arg_parser.add<std::string>("rcam", 'r', "right camera port, e.g /dev/video2", true, "");
    arg_parser.add<int>("lp", '\0', "left gpio pin, e.g 16", true, 16);
    arg_parser.add<int>("rp", '\0', "right gpio pin, e.g 18", true, 18);
    arg_parser.add<std::string>("save_path", 's', "picture save path", true, "");
}

int main(int argc, char *argv[]) {
    cmdline::parser arg_parser;
    parse_args(arg_parser);
    arg_parser.parse_check(argc, argv);

    std::string save_prefix = arg_parser.get<std::string>("save_path");
    std::string left_cam = arg_parser.get<std::string>("lcam");
    std::string right_cam = arg_parser.get<std::string>("rcam");
    int left_gpio_pin = arg_parser.get<int>("lp");
    int right_gpio_pin = arg_parser.get<int>("rp");

    HallSensor lhall = HallSensor(left_gpio_pin);
    HallSensor rhall = HallSensor(right_gpio_pin);

    lhall.open_hall();
    rhall.open_hall();

#ifdef NVV4L2DECODER
    std::string gst_string = "v4l2src device={} ! image/jpeg, width=640, height=480, framerate=60/1, format=MJPG ! nvv4l2decoder mjpeg=true ! nvvidconv ! videoconvert ! video/x-raw, format=BGR ! appsink";
#else
    std::string gst_string = "v4l2src device={} ! image/jpeg, width=640, height=480, framerate=60/1 ! jpegdec ! videoconvert ! video/x-raw, format=BGR ! appsink";
#endif
    std::string gst_left = fmt::format(gst_string, left_cam);
    std::string gst_right = fmt::format(gst_string, right_cam);
    try {
        Camera lcam = Camera();
        Camera rcam = Camera();
        lcam.open_camera(gst_left);
        rcam.open_camera(gst_right);

        std::thread process_frame_thread = std::thread(process_frame, std::ref(lcam), std::ref(rcam), std::ref(lhall), std::ref(rhall), std::ref(save_prefix));
        
        lcam.camera_thread.join();
        rcam.camera_thread.join();
        
        lhall.hall_thread.join();
        rhall.hall_thread.join();

        process_frame_thread.join();

    } catch (int e) {
        std::cout << e << std::endl;
        cv::destroyAllWindows();
    }
    return 0;
}
