#include <sys/time.h>
#include <filesystem>

#include <ctime>
#include "camera.hpp"


std::string fresh_video_name() {
    std::stringstream str_time;
    char tAll[255];
    std::time_t current_time = std::time(nullptr);
    std::strftime(tAll, sizeof(tAll), "%Y_%m_%d_%H_%M_%S", std::localtime(&current_time));
    str_time << tAll;
    return str_time.str();
}

void Camera::push_frame(cv::Mat &frame, double timestamp, time_t system_timestamp) {
    lock->lock();
    int ptr = ring_queue.tail;
    int pre = ptr - 1;
    if (pre < 0) {
        pre = RingQueue::queue_len - 1;
    }
    if (ring_queue.timestamps[pre] != timestamp) {
        ring_queue.frames[ptr] = frame;
        ring_queue.timestamps[ptr] = timestamp;
        ring_queue.system_timestamp[ptr] = system_timestamp;

        ring_queue.tail++;

        if (ring_queue.tail % RingQueue::queue_len == 0) {
            ring_queue.tail = 0;
        }
    } else {
        std::cout << "帧时间戳相同" << std::endl;
    }

    lock->unlock();
}


void start_camera(std::string &dev, Camera *camera) {
    cv::VideoCapture cap = cv::VideoCapture(dev, cv::CAP_GSTREAMER);
    cap.set(cv::CAP_PROP_BUFFERSIZE, 200);

    timeval cam_tv;
    timespec cam_tn;

    double timestamp;
    int cnt = 0;
    while (cap.isOpened()) {
        cv::Mat frame{};
        bool retVal = cap.read(frame);
        timestamp = cap.get(cv::CAP_PROP_POS_MSEC);
        gettimeofday(&cam_tv, nullptr);
//        tv.tv_usec;  // 微秒

//        clock_gettime(CLOCK_REALTIME, &tn);
//        tn.tv_nsec; // 纳秒
        if (retVal) {
            camera->push_frame(frame, timestamp, cam_tv.tv_sec * 1000000 + cam_tv.tv_usec);
        }
        cnt++;
    }
}

void Camera::open_camera(std::string &camera) {
    this->camera_thread = std::thread(start_camera, std::ref(camera), this);
}

void Camera::get_frame(FrameResult &frame_result) {
    lock->lock();
    int tail = ring_queue.tail;
    frame_result.frame = ring_queue.frames[tail];
    frame_result.timestamp = ring_queue.timestamps[tail];
    frame_result.system_timestamp = ring_queue.system_timestamp[tail];
    lock->unlock();
}


void process_frame(Camera &cam1, Camera &cam2, bool save_frame, bool show_frame) {
    std::vector<FrameResult> frame_results{};

    double pre1 = 0;
    double pre2 = 0;
    int cnt = 0;
    while (true) {
        FrameResult frameResult1{};
        FrameResult frameResult2{};

        cam1.get_frame(frameResult1);
        cam2.get_frame(frameResult2);

        double t1 = frameResult1.timestamp;
        double t2 = frameResult2.timestamp;

        if (t1 != pre1 && t2 != pre2) {
            std::cout << t1 << "--" << frameResult1.system_timestamp << std::endl;
            if (t1 > 0) {
                cv::Mat image{};
                cv::Mat arr[] = {
                        frameResult1.frame, frameResult2.frame
                };
                cv::hconcat(arr, 2, image);
//                cv::imshow("test", image);
                std::string time_str = "_" + std::to_string(frameResult1.system_timestamp) +
                                       "_" + std::to_string(frameResult2.system_timestamp) + ".jpg";
//                cv::imwrite(tmp_prefix.append(std::to_string(cnt)).append(time_str), image);
//                frame_results.emplace_back(frameResult1);

                int key = cv::waitKey(1);
                if (key == char('q')) {
                    break;
                }
                pre1 = t1;
                pre2 = t2;
                cnt += 1;
            }
        }
    }
}

void process_frame(Camera &cam, HallSensor &hall_sensor, bool save_frame, bool show_frame, std::string& dst_dir) {
    std::vector<FrameResult> frame_results{};
    double pre = 0;
    int cnt = 0;
    std::string window_name = "cam";
    if (show_frame) {
        cv::namedWindow(window_name);
        cv::moveWindow(window_name, 10, 20);
    }

    std::string video_name = fresh_video_name();

    FrameResult frameResult{};
    int hall_state = 0;
    bool door_open = false;

    while (true) {
        hall_sensor.get_hall_state(hall_state);
        door_open = hall_state;
        if (!door_open) {
            video_name = fresh_video_name();
            cnt = 0;
        }
        cam.get_frame(frameResult);
        double t1 = frameResult.timestamp;
        if (t1 != pre) {
            if (t1 > 0) {
                if (show_frame) {
                    cv::imshow(window_name, frameResult.frame);
                    int key = cv::waitKey(1);
                    if (key == char('q')) {
                        break;
                    }
                }
                
                if (save_frame && door_open) {
                    std::string video_folder = dst_dir + "/" + video_name;
                    std::cout << "door open-------" << video_folder << std::endl;
                    std::filesystem::create_directories(video_folder);
                    std::string time_str = "_" + std::to_string(frameResult.system_timestamp) +  "_" + std::to_string(cnt) + ".jpg";
                    std::string file = video_folder + "/" + video_name + time_str;
                    cv::imwrite(file, frameResult.frame);
                }
                pre = t1;
                cnt += 1;
            }
        }
    }
}