add_executable(pass_args pass_args.cpp)

include_directories(${PROJECT_SOURCE_DIR}/include)
target_compile_features(pass_args PRIVATE cxx_std_17)

target_link_libraries(pass_args PRIVATE fmt::fmt)
