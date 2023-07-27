add_executable(gpio gpio.cpp)

target_compile_features(gpio PRIVATE cxx_std_17)

target_link_libraries(gpio PRIVATE fmt::fmt JetsonGPIO::JetsonGPIO)
