cmake_minimum_required(VERSION 3.13...3.25)
project(jetson_cam_gpio)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

EXECUTE_PROCESS(COMMAND uname -m COMMAND tr -d "\n" OUTPUT_VARIABLE ARCHITECTURE)

find_package(fmt)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

if (${ARCHITECTURE} STREQUAL "aarch64")
    find_package(JetsonGPIO)
    add_subdirectory(apps/gpio)
endif ()

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(src/camera)

if (${ARCHITECTURE} STREQUAL "aarch64")
    add_subdirectory(src/hall_sensor)
endif ()

add_subdirectory(apps/mkdir)
add_subdirectory(apps/pass_args)

add_executable(${PROJECT_NAME} main.cpp)
file(GLOB sources CONFIGURE_DEPENDS *.cpp *.h *.hpp)

if (${ARCHITECTURE} STREQUAL "aarch64")
    target_compile_definitions(${PROJECT_NAME} PUBLIC NVV4L2DECODER)
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE ${Python3_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE
        fmt::fmt
        Threads::Threads
        ${OpenCV_LIBRARIES}
        camera_library
        hall_sensor_library)

if (${ARCHITECTURE} STREQUAL "aarch64")
    target_link_libraries(${PROJECT_NAME} PRIVATE JetsonGPIO::JetsonGPIO)
endif ()
