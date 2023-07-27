#include <iostream>
#include <ctime>
#include <filesystem>

#include "fmt/format.h"

int main() {

    std::string test_dir = "/data/workspace/workspace-clion/jetson_cam_gpio/apps/mkdir/tmp/1";
    std::filesystem::create_directories(test_dir);
    std::filesystem::create_directories(test_dir + "/2");
    std::filesystem::create_directories(test_dir + "/3");

    std::stringstream str_time;
    std::time_t current_time = std::time(nullptr);
    char tAll[255];
    std::strftime(tAll, sizeof(tAll), "%Y_%m_%d_%H_%M_%S", std::localtime(&current_time));
    str_time << tAll;
    std::filesystem::create_directories(test_dir + "/" + str_time.str());
    return 0;
}