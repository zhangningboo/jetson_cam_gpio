add_executable(mkdir mkdir.cpp)

target_compile_features(mkdir PRIVATE cxx_std_17)

target_link_libraries(mkdir PRIVATE fmt::fmt)
